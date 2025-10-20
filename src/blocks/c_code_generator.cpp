#include "blocks/c_code_generator.h"
#include <iomanip>
#include <limits>
#include <math.h>
#include <sstream>

namespace block {

#ifdef ENABLE_D2X
void c_code_generator::save_static_info(block::Ptr a) {
	if (!use_d2x)
		return;

	for (auto &keyval : a->static_offset.static_var_key_values) {
		xctx.set_var_here(keyval.first, keyval.second);
	}
	for (auto &addr : a->static_offset.pointers) {
		int line_no = -1;
		const char *filename = NULL;
		std::string function_name, linkage_name;
		if (d2x::util::find_line_info(addr, &line_no, &filename, function_name, linkage_name) == 0) {
			// Skip frames that are in the builder namespace
			// this can be changed to decode the name properly
			if (function_name != "" && linkage_name.rfind("_ZN7builder", 0) != 0) {
				std::string fname = filename;
				xctx.push_source_loc({fname, line_no, function_name, -1});
			}
		}
	}
}
void c_code_generator::nextl(void) {
	oss << std::endl;
	if (!use_d2x)
		return;
	xctx.nextl();
}
#else
void c_code_generator::save_static_info(block::Ptr a) {}
void c_code_generator::nextl(void) {
	oss << std::endl;
}

#endif

void c_code_generator::visit(not_expr::Ptr a) {
	oss << "!(";
	a->expr1->accept(this);
	oss << ")";
}

void c_code_generator::visit(unary_minus_expr::Ptr a) {
	oss << "-(";
	a->expr1->accept(this);
	oss << ")";
}

void c_code_generator::visit(bitwise_not_expr::Ptr a) {
	oss << "~(";
	a->expr1->accept(this);
	oss << ")";
}

static bool expr_needs_bracket(expr::Ptr a) {
	if (isa<binary_expr>(a))
		return true;
	else if (isa<assign_expr>(a))
		return true;
	return false;
}
void c_code_generator::emit_binary_expr(binary_expr::Ptr a, std::string character) {
	if (expr_needs_bracket(a->expr1)) {
		oss << "(";
		a->expr1->accept(this);
		oss << ")";
	} else
		a->expr1->accept(this);
	oss << " " << character << " ";
	if (expr_needs_bracket(a->expr2)) {
		oss << "(";
		a->expr2->accept(this);
		oss << ")";
	} else
		a->expr2->accept(this);
}
void c_code_generator::visit(and_expr::Ptr a) {
	emit_binary_expr(a, "&&");
}
void c_code_generator::visit(bitwise_and_expr::Ptr a) {
	emit_binary_expr(a, "&");
}
void c_code_generator::visit(or_expr::Ptr a) {
	emit_binary_expr(a, "||");
}
void c_code_generator::visit(bitwise_or_expr::Ptr a) {
	emit_binary_expr(a, "|");
}
void c_code_generator::visit(bitwise_xor_expr::Ptr a) {
	emit_binary_expr(a, "^");
}
void c_code_generator::visit(plus_expr::Ptr a) {
	emit_binary_expr(a, "+");
}
void c_code_generator::visit(minus_expr::Ptr a) {
	emit_binary_expr(a, "-");
}
void c_code_generator::visit(mul_expr::Ptr a) {
	emit_binary_expr(a, "*");
}
void c_code_generator::visit(div_expr::Ptr a) {
	emit_binary_expr(a, "/");
}
void c_code_generator::visit(lt_expr::Ptr a) {
	emit_binary_expr(a, "<");
}
void c_code_generator::visit(gt_expr::Ptr a) {
	emit_binary_expr(a, ">");
}
void c_code_generator::visit(lte_expr::Ptr a) {
	emit_binary_expr(a, "<=");
}
void c_code_generator::visit(gte_expr::Ptr a) {
	emit_binary_expr(a, ">=");
}
void c_code_generator::visit(lshift_expr::Ptr a) {
	emit_binary_expr(a, "<<");
}
void c_code_generator::visit(rshift_expr::Ptr a) {
	emit_binary_expr(a, ">>");
}
void c_code_generator::visit(equals_expr::Ptr a) {
	emit_binary_expr(a, "==");
}
void c_code_generator::visit(ne_expr::Ptr a) {
	emit_binary_expr(a, "!=");
}
void c_code_generator::visit(mod_expr::Ptr a) {
	emit_binary_expr(a, "%");
}
void c_code_generator::visit(var_expr::Ptr a) {
	oss << a->var1->var_name;
}
void c_code_generator::visit(int_const::Ptr a) {
	oss << a->value;
	if (a->is_64bit)
		oss << "ll";
}
void c_code_generator::visit(double_const::Ptr a) {
	oss << std::setprecision(15);
	oss << a->value;
	if (floor(a->value) == a->value)
		oss << ".0";
}
void c_code_generator::visit(float_const::Ptr a) {
	oss << std::setprecision(15);
	oss << a->value;
	if (floor(a->value) == a->value)
		oss << ".0";
	oss << "f";
}
void c_code_generator::visit(string_const::Ptr a) {
	oss << "\"" << a->value << "\"";
}
void c_code_generator::visit(assign_expr::Ptr a) {
	if (expr_needs_bracket(a->var1)) {
		oss << "(";
		a->var1->accept(this);
		oss << ")";
	} else
		a->var1->accept(this);

	oss << " = ";
	a->expr1->accept(this);
}

void c_code_generator::visit(expr_stmt::Ptr a) {
	a->expr1->accept(this);
	oss << ";";
	if (!a->annotation.empty()) {
		oss << " //";
		for (auto s: a->annotation)
		oss << s << " ";
	}
}

void c_code_generator::print_pragma(stmt::Ptr s) {
	static std::string pragma_prefix ("pragma: ");
	for (auto a: s->annotation) {
		if (!a.compare(0, pragma_prefix.size(), pragma_prefix)) {
			std::string pragma_value = a.substr(pragma_prefix.size());
			printer::indent(oss, curr_indent);
			oss << "_Pragma(\"" << pragma_value << "\")" << std::endl;
		}
	}
}

void c_code_generator::visit(stmt_block::Ptr a) {
	oss << "{";
	nextl();
	curr_indent += 1;
	for (auto stmt : a->stmts) {
		print_pragma(stmt);
		printer::indent(oss, curr_indent);
		stmt->accept(this);
		save_static_info(stmt);
		nextl();
	}
	curr_indent -= 1;
	printer::indent(oss, curr_indent);

	oss << "}";
}


static std::string addQualifiers(type::Ptr a, bool before = true) {
	std::string ret = "";
	if (before) {
		if (a->is_const) ret += "const ";
		if (a->is_volatile) ret += "volatile ";
	} else {
		if (a->is_const) ret += " const";
		if (a->is_volatile) ret += " volatile";
	}
	return ret;
}
static std::string complex_type_helper(type::Ptr a, std::string name) {
	if (isa<pointer_type>(a)) {
		auto ptr = to<pointer_type>(a);
		std::string decorated = "*" + addQualifiers(ptr, false) + name;
		if (isa<function_type>(ptr->pointee_type) || isa<array_type>(ptr->pointee_type)) {
			decorated = " (" + decorated + ")";
		}
		return complex_type_helper(ptr->pointee_type, decorated);
	} else if (isa<array_type>(a)) {
		auto arr = to<array_type>(a);
		std::string ret = name + "[";
		if (arr->size != -1)
			ret += std::to_string(arr->size);
		ret += "]";
		return complex_type_helper(arr->element_type, ret);
	} else if (isa<function_type>(a)) {
		auto ft = to<function_type>(a);
		std::string ret = name + "(";
		for (unsigned int i = 0; i < ft->arg_types.size(); i++) {
			ret += complex_type_helper(ft->arg_types[i], "");
			if (i + 1 < ft->arg_types.size()) 
				ret += ", ";
		}
		ret += ")";
		return complex_type_helper(ft->return_type, ret);
	} else if (isa<scalar_type>(a)) {
		auto st = to<scalar_type>(a);
		std::string tp;
		switch (st->scalar_type_id) {
			case scalar_type::SHORT_INT_TYPE:
				tp = "short int";
				break;
			case scalar_type::UNSIGNED_SHORT_INT_TYPE:
				tp = "unsigned short int";
				break;
			case scalar_type::INT_TYPE:
				tp = "int";
				break;
			case scalar_type::UNSIGNED_INT_TYPE:
				tp = "unsigned int";
				break;
			case scalar_type::LONG_INT_TYPE:
				tp = "long int";
				break;
			case scalar_type::UNSIGNED_LONG_INT_TYPE:
				tp = "unsigned long int";
				break;
			case scalar_type::LONG_LONG_INT_TYPE:
				tp = "long long int";
				break;
			case scalar_type::UNSIGNED_LONG_LONG_INT_TYPE:
				tp = "unsigned long long int";
				break;
			case scalar_type::CHAR_TYPE:
				tp = "char";
				break;
			case scalar_type::UNSIGNED_CHAR_TYPE:
				tp = "unsigned char";
				break;
			case scalar_type::VOID_TYPE:
				tp = "void";
				break;
			case scalar_type::FLOAT_TYPE:
				tp = "float";
				break;
			case scalar_type::DOUBLE_TYPE:
				tp = "double";
				break;
			case scalar_type::BOOL_TYPE:
				tp = "bool";
				break;
			default:
				assert(false && "Invalid scalar type");
		}
		return addQualifiers(st) + tp + name;
	} else if (isa<named_type>(a)) {
		auto nt = to<named_type>(a);
		std::string ret = addQualifiers(nt) + nt->type_name;
		if (nt->template_args.size()) {
			ret += "<";
			bool needs_comma = false;
			for (auto a : nt->template_args) {
				if (needs_comma)
					ret += ", ";
				needs_comma = true;
				ret += complex_type_helper(a, "");
			}
			ret += ">";
		}
		return ret + name;
	} else if (isa<reference_type>(a)) {
		auto ptr = to<reference_type>(a);
		std::string decorated = "&" + addQualifiers(ptr, false) + name;
		if (isa<function_type>(ptr->referenced_type) || isa<array_type>(ptr->referenced_type)) {
			decorated = " (" + decorated + ")";
		}
		return complex_type_helper(ptr->referenced_type, decorated);
	} else if (isa<builder_var_type>(a)) {
		auto bt = to<builder_var_type>(a);
		std::string ret = addQualifiers(bt);
		if (bt->builder_var_type_id == builder_var_type::DYN_VAR)
			ret += "builder::dyn_var<";
		else if (bt->builder_var_type_id == builder_var_type::STATIC_VAR)
			ret += "builder::static_var<";
		ret += complex_type_helper(bt->closure_type, "");
		ret += ">";
		return ret + name;
	}
	a->dump(std::cerr, 0);
	assert(false && "Invalid type");
}

void c_code_generator::visit(scalar_type::Ptr type) {
	oss << complex_type_helper(type, "");
}
void c_code_generator::visit(named_type::Ptr type) {
	oss << complex_type_helper(type, "");
}
void c_code_generator::visit(pointer_type::Ptr type) {
	oss << complex_type_helper(type, "");
}
void c_code_generator::visit(reference_type::Ptr type) {
	oss << complex_type_helper(type, "");
}
void c_code_generator::visit(array_type::Ptr type) {
	oss << complex_type_helper(type, "");
}
void c_code_generator::visit(builder_var_type::Ptr type) {
	oss << complex_type_helper(type, "");
}
void c_code_generator::visit(var::Ptr var) {
	oss << var->var_name;
}

void c_code_generator::visit(decl_stmt::Ptr a) {
	oss << complex_type_helper(a->decl_var->var_type, " " + a->decl_var->var_name);
	if (a->decl_var->hasMetadata<std::vector<std::string>>("attributes")) {
		const auto &attributes = a->decl_var->getMetadata<std::vector<std::string>>("attributes");
		for (auto attr : attributes) {
			oss << " " << attr;
		}
	}
	if (a->init_expr == nullptr) {
		oss << ";";
	} else {
		oss << " = ";
		a->init_expr->accept(this);
		oss << ";";
	}
}
void c_code_generator::visit(if_stmt::Ptr a) {
	oss << "if (";
	a->cond->accept(this);
	oss << ")";
	if (isa<stmt_block>(a->then_stmt)) {
		oss << " ";
		a->then_stmt->accept(this);
		oss << " ";
	} else {
		save_static_info(a);
		nextl();
		curr_indent++;
		printer::indent(oss, curr_indent);
		a->then_stmt->accept(this);
		save_static_info(a);
		nextl();
		curr_indent--;
	}

	if (isa<stmt_block>(a->else_stmt)) {
		if (to<stmt_block>(a->else_stmt)->stmts.size() == 0)
			return;
		oss << "else";
		oss << " ";
		a->else_stmt->accept(this);
	} else {
		oss << "else";
		save_static_info(a);
		nextl();
		curr_indent++;
		printer::indent(oss, curr_indent);
		a->else_stmt->accept(this);
		curr_indent--;
	}
}
void c_code_generator::visit(while_stmt::Ptr a) {
	oss << "while (";
	a->cond->accept(this);
	oss << ")";
	if (isa<stmt_block>(a->body)) {
		save_static_info(a);
		oss << " ";
		a->body->accept(this);
	} else {
		save_static_info(a);
		nextl();
		curr_indent++;
		printer::indent(oss, curr_indent);
		a->body->accept(this);
		curr_indent--;
	}
}
void c_code_generator::visit(for_stmt::Ptr a) {
	oss << "for (";
	a->decl_stmt->accept(this);
	oss << " ";
	a->cond->accept(this);
	oss << "; ";
	a->update->accept(this);
	oss << ")";
	if (isa<stmt_block>(a->body)) {
		save_static_info(a);
		oss << " ";
		a->body->accept(this);
	} else {
		save_static_info(a);
		nextl();
		curr_indent++;
		printer::indent(oss, curr_indent);
		a->body->accept(this);
		curr_indent--;
	}
}
void c_code_generator::visit(break_stmt::Ptr a) {
	oss << "break;";
}
void c_code_generator::visit(continue_stmt::Ptr a) {
	oss << "continue;";
}
void c_code_generator::visit(sq_bkt_expr::Ptr a) {
	if (expr_needs_bracket(a->var_expr)) {
		oss << "(";
	}
	a->var_expr->accept(this);
	if (expr_needs_bracket(a->var_expr)) {
		oss << ")";
	}
	oss << "[";
	a->index->accept(this);
	oss << "]";
}
void c_code_generator::visit(function_call_expr::Ptr a) {
	if (expr_needs_bracket(a->expr1)) {
		oss << "(";
	}
	a->expr1->accept(this);
	if (expr_needs_bracket(a->expr1)) {
		oss << ")";
	}
	oss << "(";
	for (unsigned int i = 0; i < a->args.size(); i++) {
		a->args[i]->accept(this);
		if (i != a->args.size() - 1)
			oss << ", ";
	}
	oss << ")";
}
void c_code_generator::visit(initializer_list_expr::Ptr a) {
	oss << "{";
	for (unsigned int i = 0; i < a->elems.size(); i++) {
		a->elems[i]->accept(this);
		if (i != a->elems.size() - 1)
			oss << ", ";
	}
	oss << "}";
}
void c_code_generator::handle_func_arg(var::Ptr a) {
	function_type::Ptr type = to<function_type>(a->var_type);
	type->return_type->accept(this);
	oss << " (*";
	oss << a->var_name;
	oss << ")(";
	for (unsigned int i = 0; i < type->arg_types.size(); i++) {
		type->arg_types[i]->accept(this);
		if (i != type->arg_types.size() - 1)
			oss << ", ";
	}
	oss << ")";
	return;
}
void c_code_generator::visit(func_decl::Ptr a) {

#ifdef ENABLE_D2X
	if (use_d2x) {
		oss << xctx.begin_section();
	}
#endif

	printer::indent(oss, curr_indent);
	a->return_type->accept(this);
	if (a->hasMetadata<std::vector<std::string>>("attributes")) {
		const auto &attributes = a->getMetadata<std::vector<std::string>>("attributes");
		for (auto attr : attributes) {
			oss << " " << attr;
		}
	}
	oss << " " << a->func_name;
	oss << " (";
	bool printDelim = false;
	for (auto arg : a->args) {
		if (printDelim)
			oss << ", ";
		printDelim = true;
		oss << complex_type_helper(arg->var_type, " " + arg->var_name);
	}
	if (!printDelim)
		oss << "void";
	oss << ")";
	if (decl_only) {
		oss << ";";
		return;
	}
	if (isa<stmt_block>(a->body)) {
		oss << " ";
		a->body->accept(this);
		save_static_info(a);
		nextl();
	} else {
		oss << std::endl;
		curr_indent++;
		printer::indent(oss, curr_indent);
		a->body->accept(this);
		save_static_info(a);
		nextl();
		curr_indent--;
	}
#ifdef ENABLE_D2X
	if (use_d2x) {
		xctx.emit_function_info(oss);
		xctx.end_section();
	}
#endif
}
void c_code_generator::visit(struct_decl::Ptr a) {
	printer::indent(oss, curr_indent);
	oss << "struct " << a->struct_name << " {";
	nextl();
	curr_indent++;
	for (auto mem: a->members) {
		printer::indent(oss, curr_indent);
		mem->accept(this);
		nextl();
	}
	curr_indent--;
	printer::indent(oss, curr_indent);
	oss << "};";
	nextl();
}
void c_code_generator::visit(goto_stmt::Ptr a) {
	// a->dump(oss, 1);
	oss << "goto ";
	oss << a->label1->label_name << ";";
}
void c_code_generator::visit(label_stmt::Ptr a) {
	oss << a->label1->label_name << ":";
}
void c_code_generator::visit(return_stmt::Ptr a) {
	oss << "return ";
	a->return_val->accept(this);
	oss << ";";
}
void c_code_generator::visit(member_access_expr::Ptr a) {
	if (isa<sq_bkt_expr>(a->parent_expr)) {
		sq_bkt_expr::Ptr parent = to<sq_bkt_expr>(a->parent_expr);
		if (parent->getBoolMetadata("deref_is_star")) {
			if (isa<int_const>(parent->index)) {
				auto index = to<int_const>(parent->index);
				if (index->value == 0) {
					if (!isa<var_expr>(parent->var_expr)) {
						oss << "(";
					}
					parent->var_expr->accept(this);
					if (!isa<var_expr>(parent->var_expr)) {
						oss << ")";
					}
					oss << "->" << a->member_name;
					return;
				}
			}
		}
	}

	if (!isa<var_expr>(a->parent_expr))
		oss << "(";
	a->parent_expr->accept(this);
	if (!isa<var_expr>(a->parent_expr))
		oss << ")";

	oss << "." << a->member_name;
}
void c_code_generator::visit(addr_of_expr::Ptr a) {
	oss << "(&(";
	a->expr1->accept(this);
	oss << "))";
}
} // namespace block
