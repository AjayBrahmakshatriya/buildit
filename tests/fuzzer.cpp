#include "blocks/stmt.h"
#include "blocks/expr.h"
#include "blocks/c_code_generator.h"
#include "blocks/if_switcher.h"
using namespace block;

static int gen_random(int range) {
	return random() % range;
}

struct sctx {
	std::vector<var::Ptr> var_set;
	int max_depth = 3;	
	int n_decls = 5;
};

/* Possible statements 

1. simple assign stmt
2. if condition with or without else
3. while loop 
4. break stmt
5. continue stmt

*/

static expr::Ptr sample_variable(const sctx &ctx) {
	var_expr::Ptr ve = std::make_shared<var_expr>();	
	ve->var1 = ctx.var_set[gen_random(ctx.var_set.size())];
	return ve;
}

static expr::Ptr generate_assignment(const sctx &ctx) {
	expr::Ptr lhs = sample_variable(ctx);
	expr::Ptr rhs = sample_variable(ctx);

	plus_expr::Ptr rhs_add = std::make_shared<plus_expr>();
	rhs_add->expr1 = rhs;
	int_const::Ptr rhs_const = std::make_shared<int_const>();
	rhs_const->value = gen_random(100);
	rhs_add->expr2 = rhs_const;

	assign_expr::Ptr ae = std::make_shared<assign_expr>();
	ae->var1 = lhs;
	ae->expr1 = rhs_add;

	return ae;
}

static binary_expr::Ptr gen_comparison_operator(void) {
	switch(gen_random(6)) {
		case 0: return std::make_shared<equals_expr>();
		case 1: return std::make_shared<ne_expr>();
		case 2: return std::make_shared<lt_expr>();
		case 3: return std::make_shared<lte_expr>();
		case 4: return std::make_shared<gt_expr>();
		case 5: return std::make_shared<gte_expr>();	
	}	
	return nullptr;
}

static expr::Ptr generate_comparison(const sctx& ctx) {
	expr::Ptr lhs = sample_variable(ctx);
	expr::Ptr rhs = sample_variable(ctx);
	
	binary_expr::Ptr be = gen_comparison_operator();
	be->expr1 = lhs;
	be->expr2 = rhs;
	return be;	
}


static if_stmt::Ptr generate_if(const sctx& ctx, int depth, bool inloop);
static stmt::Ptr generate_while(const sctx& ctx, int depth, bool inloop);

static continue_stmt::Ptr generate_continue(void) {
	return std::make_shared<continue_stmt>();
}
static break_stmt::Ptr generate_break(void) {
	return std::make_shared<break_stmt>();
}

static expr_stmt::Ptr expr_from_stmt(expr::Ptr e) {
	expr_stmt::Ptr es = std::make_shared<expr_stmt>();
	es->expr1 = e;
	return es;
}

static stmt_block::Ptr generate_body(const sctx& ctx, int depth, bool inloop) {
	unsigned int num_stmts = gen_random(3) + 1;
	stmt_block::Ptr bs = std::make_shared<stmt_block>();

	if_stmt::Ptr new_if;

	while (bs->stmts.size() < num_stmts) {
		switch (gen_random(5)) {
			case 0: bs->stmts.push_back(expr_from_stmt(generate_assignment(ctx))); break;
			case 1: if (depth > ctx.max_depth) continue; 
				new_if = generate_if(ctx, depth, inloop);
				bs->stmts.push_back(new_if);
				// Cleanup if conditions that BOTH end in continue
				if (to<stmt_block>(new_if->then_stmt)->stmts.size() > 0 && to<stmt_block>(new_if->else_stmt)->stmts.size() > 0) {
					if (isa<continue_stmt>(to<stmt_block>(new_if->then_stmt)->stmts.back()) &&
						isa<continue_stmt>(to<stmt_block>(new_if->else_stmt)->stmts.back())) {
						bs->stmts.push_back(std::make_shared<continue_stmt>());
						to<stmt_block>(new_if->then_stmt)->stmts.pop_back();
						to<stmt_block>(new_if->else_stmt)->stmts.pop_back();
						return bs;	
					}
				}
				break;
			case 2: if (depth > ctx.max_depth) continue; bs->stmts.push_back(generate_while(ctx, depth, true)); break;
			case 3: if (!inloop) continue; bs->stmts.push_back(generate_break()); return bs;
			case 4: if (!inloop) continue; bs->stmts.push_back(generate_continue()); return bs;
		}
	}
	return bs;
}

static if_stmt::Ptr generate_if(const sctx& ctx, int depth, bool inloop) {
	if_stmt::Ptr is = std::make_shared<if_stmt>();
	is->cond = generate_comparison(ctx);
	is->then_stmt = generate_body(ctx, depth + 1, inloop);
	if (gen_random(2)) {
		is->else_stmt = generate_body(ctx, depth + 1, inloop);
	} else {
		is->else_stmt = std::make_shared<stmt_block>();
	}
	return is;
}

static void remove_trailing_continues(stmt_block::Ptr sb) {
	if (sb->stmts.size() == 0) return;
	if (isa<continue_stmt>(sb->stmts.back())) {
		sb->stmts.pop_back();
		return;
	}
	if (isa<if_stmt>(sb->stmts.back())) {
		auto is = to<if_stmt>(sb->stmts.back());
		remove_trailing_continues(to<stmt_block>(is->then_stmt));
		remove_trailing_continues(to<stmt_block>(is->else_stmt));
		return;
	}
}

class break_continue_counter: public block_visitor {
public:
	int break_count = 0;
	int continue_count = 0;
	using block_visitor::visit;
	virtual void visit(break_stmt::Ptr) {
		break_count++;
	}
	virtual void visit(continue_stmt::Ptr) {
		continue_count++;
	}

	// Do not recurse for loops
	virtual void visit(while_stmt::Ptr) {
		return;
	}
	
};

static void fixup_dangling_continues(stmt_block::Ptr sb) {
	for (unsigned int i = 0; i < sb->stmts.size(); i++) {
		auto stmt = sb->stmts[i];
		if (isa<if_stmt>(stmt)) {
			auto ifs = to<if_stmt>(stmt);
			if (to<stmt_block>(ifs->then_stmt)->stmts.size() > 0 
				&& (isa<continue_stmt>(to<stmt_block>(ifs->then_stmt)->stmts.back())
				|| isa<break_stmt>(to<stmt_block>(ifs->then_stmt)->stmts.back()))) {

				if (isa<continue_stmt>(to<stmt_block>(ifs->then_stmt)->stmts.back())) 
					to<stmt_block>(ifs->then_stmt)->stmts.pop_back();

				unsigned int counter = 0;
				for (i++; i < sb->stmts.size(); i++) {
					to<stmt_block>(ifs->else_stmt)->stmts.push_back(sb->stmts[i]);
					counter++;
				}
				for (unsigned j = 0; j < counter; j++) {
					sb->stmts.pop_back();
				}
				break;
			}
		}
	}
}

static void merge_trailing_breaks(stmt_block::Ptr sb) {

	std::vector<stmt::Ptr> new_stmts;

	for (unsigned int i = 0; i < sb->stmts.size(); i++) {
		auto stmt = sb->stmts[i];
		new_stmts.push_back(stmt);
		if (isa<if_stmt>(stmt)) {
			auto then_block = to<stmt_block>(to<if_stmt>(stmt)->then_stmt);	
			auto else_block = to<stmt_block>(to<if_stmt>(stmt)->else_stmt);	
			if (then_block->stmts.size() > 0 && else_block->stmts.size() > 0) {
				if (isa<break_stmt>(then_block->stmts.back()) && isa<break_stmt>(else_block->stmts.back())) {
					then_block->stmts.pop_back();
					else_block->stmts.pop_back();
					new_stmts.push_back(std::make_shared<break_stmt>());
				}
				else if (isa<continue_stmt>(then_block->stmts.back()) && isa<continue_stmt>(else_block->stmts.back())) {
					then_block->stmts.pop_back();
					else_block->stmts.pop_back();
					new_stmts.push_back(std::make_shared<continue_stmt>());
				}
			}
		}	
	}
	sb->stmts = new_stmts;
}


static stmt::Ptr generate_while(const sctx& ctx, int depth, bool inloop) {
	while_stmt::Ptr ws = std::make_shared<while_stmt>();
	ws->cond = generate_comparison(ctx);
	ws->body = generate_body(ctx, depth + 1, true);


	stmt_block::Ptr stmts = to<stmt_block>(ws->body);
	// Canonicalization of a loop body
	// Check if any of the body statements are if conditions
	// where the then block ends in continue
	
	fixup_dangling_continues(stmts);

	// Special fixup! If a while loop has only one statement and that 
	// stmt is a break, convert the while loop to an if
	// if it is a continue, remove it
	
	// This reduces number of false negatives

	remove_trailing_continues(stmts);

	merge_trailing_breaks(stmts);

	if (stmts->stmts.size() == 0)
		return ws;

	if (isa<break_stmt>(stmts->stmts.back())) {
		// make sure this while doesn't have any other continues or breaks	

		break_continue_counter counter;
		stmts->accept(&counter);
		if (counter.break_count != 1 || counter.continue_count != 0)
			return ws;

		if_stmt::Ptr is = std::make_shared<if_stmt>();
		is->cond = ws->cond;
		is->then_stmt = stmts;
		stmts->stmts.pop_back();
		is->else_stmt = std::make_shared<stmt_block>();
		return is;
	
	}
	
	return ws;
}


static void gen_decls(sctx& ctx, func_decl::Ptr fd) {
	for (int i = 0; i < ctx.n_decls; i++) {
		var::Ptr v = std::make_shared<var>();
		v->var_name = "var" + std::to_string(i);


		scalar_type::Ptr st = std::make_shared<scalar_type>();
		st->scalar_type_id = scalar_type::INT_TYPE;


		v->var_type = st;
		ctx.var_set.push_back(v);
		
		decl_stmt::Ptr ds = std::make_shared<decl_stmt>();
		ds->decl_var = v;
		int_const::Ptr rhs_const = std::make_shared<int_const>();
		rhs_const->value = gen_random(100);
		ds->init_expr = rhs_const;

		to<stmt_block>(fd->body)->stmts.push_back(ds);
	}
}

static func_decl::Ptr generate_function(std::string name, sctx& ctx) {
	func_decl::Ptr fd = std::make_shared<func_decl>();
	fd->body = std::make_shared<stmt_block>();
	fd->func_name = name;
	scalar_type::Ptr st = std::make_shared<scalar_type>();
	st->scalar_type_id = scalar_type::VOID_TYPE;
	fd->return_type = st;

	gen_decls(ctx, fd);

	stmt_block::Ptr sb = generate_body(ctx, 0, false);
	
	for (auto s: sb->stmts) {
		to<stmt_block>(fd->body)->stmts.push_back(s);	
	}

	return fd;
}


static void promote_types(const sctx& ctx) {
	for (auto v: ctx.var_set) {
		builder_var_type::Ptr bvt = std::make_shared<builder_var_type>();
		bvt->builder_var_type_id = builder_var_type::DYN_VAR;
		bvt->closure_type = v->var_type;
		v->var_type = bvt;
	}
}




int main(int argc, char* argv[]) {
	if (argc > 1) 
		srandom(atoi(argv[1]));
	else 
		srandom(42);
	sctx ctx;
	func_decl::Ptr fd = generate_function("test", ctx);	
	if_switcher switcher;
	fd->accept(&switcher);
	c_code_generator::generate_code(fd, std::cerr, 0);
	promote_types(ctx);

	std::cout << "#include \"builder/builder_context.h\"" << std::endl;	
	std::cout << "#include \"blocks/c_code_generator.h\"" << std::endl;	
	std::cout << "void test(void);" << std::endl;
	c_code_generator::generate_code(fd, std::cout, 0);
	std::cout << "int main(int argc, char* argv[]) {" << std::endl;
	std::cout << "  auto ast = builder::builder_context().extract_function_ast(test, \"test\");" << std::endl;
	std::cout << "  block::c_code_generator::generate_code(ast, std::cout, 0);" << std::endl;
	std::cout << "}" << std::endl;
	
	return 0;
}
