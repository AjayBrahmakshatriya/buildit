#include "blocks/loop_finder.h"
#include "blocks/if_flattener.h"
#include "blocks/c_code_generator.h"
#include <algorithm>
namespace block {


// This function checks the end blocks of the body and checks if they have a goto that goes to the beginning, 
// If it does, it removes it since it will be an implicit continue. For the blocks that do not go back, 
// it gathers it into break_parents
static void ensure_back_has_goto(stmt_block::Ptr a, label::Ptr label_detect, std::vector<stmt_block::Ptr> &break_parents, 
		std::vector<stmt_block::Ptr>& implicit_continue_blocks) {	
	if (a->stmts.size() == 0) {
		break_parents.push_back(a);
		return;
	}
	stmt::Ptr last_stmt = a->stmts.back();
	if (isa<if_stmt>(last_stmt)) {
		// For ifs don't add unnecessary common ends
		if_stmt::Ptr if_stmt_ptr = to<if_stmt>(last_stmt);

		stmt_block::Ptr then_block = to<stmt_block>(if_stmt_ptr->then_stmt);
		stmt_block::Ptr else_block = to<stmt_block>(if_stmt_ptr->else_stmt);
		ensure_back_has_goto(then_block, label_detect, break_parents, implicit_continue_blocks);
		ensure_back_has_goto(else_block, label_detect, break_parents, implicit_continue_blocks);
	} else if (isa<goto_stmt>(last_stmt) && to<goto_stmt>(last_stmt)->label1 == label_detect) {
		a->stmts.pop_back();
		implicit_continue_blocks.push_back(a);
	} else if (isa<goto_stmt>(last_stmt) && to<goto_stmt>(last_stmt)->label1 != label_detect) {
		break_parents.push_back(a);
	//} else if (isa<return_stmt>(last_stmt)) {
		// Do nothing for return statements, 
		// they are neither breaks nor continues
	} else if (isa<break_stmt>(last_stmt)) {
		assert(false);
	} else {
		break_parents.push_back(a);
	}
	return;
}

static void insert_continues(stmt_block::Ptr a, label::Ptr label_detect, std::vector<stmt_block::Ptr> &collect) {
	for (auto stmt : a->stmts) {
		if (isa<if_stmt>(stmt)) {
			if_stmt::Ptr if_stmt_ptr = to<if_stmt>(stmt);
			stmt_block::Ptr then_block = to<stmt_block>(if_stmt_ptr->then_stmt);
			stmt_block::Ptr else_block = to<stmt_block>(if_stmt_ptr->else_stmt);
			insert_continues(then_block, label_detect, collect);
			insert_continues(else_block, label_detect, collect);
		}
	}
	if (a->stmts.size() > 0 && isa<goto_stmt>(a->stmts.back())) {
		if (to<goto_stmt>(a->stmts.back())->label1 == label_detect) {
			a->stmts.pop_back();
			continue_stmt::Ptr cont = std::make_shared<continue_stmt>();
			a->stmts.push_back(cont);
			collect.push_back(a);
		}
	}
}
static void insert_breaks(stmt_block::Ptr a, label::Ptr label_detect, std::vector<stmt_block::Ptr> &parents) {
	for (auto stmt : a->stmts) {
		if (isa<if_stmt>(stmt)) {
			if_stmt::Ptr if_stmt_ptr = to<if_stmt>(stmt);
			stmt_block::Ptr then_block = to<stmt_block>(if_stmt_ptr->then_stmt);
			stmt_block::Ptr else_block = to<stmt_block>(if_stmt_ptr->else_stmt);
			insert_breaks(then_block, label_detect, parents);
			insert_breaks(else_block, label_detect, parents);
		}
	}

	if (a->stmts.size() == 0)
		return;

	if (a->stmts.size() > 0 && isa<goto_stmt>(a->stmts.back())) {
		if (to<goto_stmt>(a->stmts.back())->label1 == label_detect) {
			return;
		}
	}
	// This needs a break because it doesn't continue
	// But before we add, we should check if parents already has it

	if (a->stmts.size() > 0 && isa<goto_stmt>(a->stmts.back())) {
		for (auto stmt : parents) {
			if (stmt == a)
				return;
		}
		parents.push_back(a);
	}
}

void continue_finder::visit(continue_stmt::Ptr) {
	has_continue = true;
}

void continue_finder::visit(break_stmt::Ptr) {
	has_break = true;
}

static bool check_last_choppable(std::vector<stmt_block::Ptr> &parents) {
	// Check if everyone has atleast one stmt
	for (unsigned int i = 0; i < parents.size(); i++) {
		if (parents[i]->stmts.size() == 0)
			return false;
	}

	stmt::Ptr last_stmt = parents[0]->stmts.back();
	continue_finder finder;
	last_stmt->accept(&finder);
	if (finder.has_continue)
		return false;

	if (parents.size() == 1)
		return true;

	tracer::tag first_tag = parents[0]->stmts.back()->static_offset;
	for (unsigned int i = 1; i < parents.size(); i++) {
		if (parents[i]->stmts.back()->static_offset != first_tag)
			return false;
	}
	return true;
}
static void trim_from_parents(std::vector<stmt_block::Ptr> &parents, std::vector<stmt::Ptr> &trimmed) {
	// First check if the ends are all same
	if (check_last_choppable(parents)) {
		// Chop a stmt off of everyone
		stmt::Ptr chopped = parents[0]->stmts.back();
		for (unsigned int i = 0; i < parents.size(); i++) {
			parents[i]->stmts.pop_back();
		}
		trimmed.push_back(chopped);
		trim_from_parents(parents, trimmed);
	}
}

void loop_finder::visit(stmt_block::Ptr a) {

	// We do this inside out, first do the innermost loop

	// Visit the instructions normally
	for (auto stmt : a->stmts) {
		stmt->accept(this);
	}

	// Check if this block has a label
	while (1) {
		label_stmt::Ptr found_label = nullptr;
		for (auto stmt : a->stmts) {
			if (isa<label_stmt>(stmt)) {
				found_label = to<label_stmt>(stmt);
			}
		}
		if (found_label == nullptr)
			break;
		visit_label(found_label, a);
	}
}

static stmt::Ptr fix_loop_inversion_impl(if_stmt::Ptr ifs) {

	if (!isa<stmt_block>(ifs->then_stmt))
		return ifs;
	
	auto then_block = to<stmt_block>(ifs->then_stmt);
	auto else_block = to<stmt_block>(ifs->else_stmt);
	
	if (then_block->stmts.size() != 1 || else_block->stmts.size() != 0) 
		return ifs;

	if (!isa<while_stmt>(then_block->stmts[0]))
		return ifs;
	
	auto ws = to<while_stmt>(then_block->stmts[0]);

	if (!isa<int_const>(ws->cond))
		return ifs;

	if (!(to<int_const>(ws->cond)->value == 1))
		return ifs;

	if (!isa<stmt_block>(ws->body))
		return ifs;
	
	if (to<stmt_block>(ws->body)->stmts.size() == 0) 
		return ifs;
	
	auto last_stmt = to<stmt_block>(ws->body)->stmts.back();
	if (!isa<if_stmt>(last_stmt))
		return ifs;

	if (!isa<not_expr>(to<if_stmt>(last_stmt)->cond))
		return ifs;
	if (!to<not_expr>(to<if_stmt>(last_stmt)->cond)->expr1->is_same(ifs->cond))
		return ifs;

	auto nfs = to<if_stmt>(last_stmt);

	if (!isa<stmt_block>(nfs->then_stmt) || to<stmt_block>(nfs->then_stmt)->stmts.size() != 1)
		return ifs;
	if (!isa<stmt_block>(nfs->else_stmt) || to<stmt_block>(nfs->else_stmt)->stmts.size() != 0)
		return ifs;

	if (!isa<break_stmt>(to<stmt_block>(nfs->then_stmt)->stmts[0]))
		return ifs;	
	
	// make sure ws has no continues
	continue_finder counter;
	ws->accept(&counter);
	if (counter.has_continue)
		return ifs;


	// everything looks good, time to patch
	ws->cond = ifs->cond;
	to<stmt_block>(ws->body)->stmts.pop_back();
	ws->implicit_continue_blocks = {to<stmt_block>(ws->body)};
	
	return ws;
}

void fix_loop_inversion::visit(if_stmt::Ptr ifs) {
	block_replacer::visit(ifs);	
	node = fix_loop_inversion_impl(ifs);
}


static void merge_condition_with_loop(while_stmt::Ptr new_while, std::vector<stmt::Ptr> &stmts_before) {
	//return;
	// If the body of the while loop only has a single if condition and
	// the else part of the condition is just a break, fuse the if with
	// the loop

	// Another pattern is the do-while pattern where if the loop has multiple statements and the last
	// statement is like the one described above, and the statements before don't have break or continues, 
	// it can be turned into a loop too

	// Also accepts a reference to statements before, required for do-while conversion

	if (to<stmt_block>(new_while->body)->stmts.size() == 1 &&
	    isa<if_stmt>(to<stmt_block>(new_while->body)->stmts.back())) {
		if_stmt::Ptr if_body = to<if_stmt>(to<stmt_block>(new_while->body)->stmts.back());

		// Make sure the statements before don't have a break or continue
		bool prior_is_eligible = true;
		auto sb = to<stmt_block>(new_while->body);
		for (unsigned i = 0; i < sb->stmts.size() - 1; i++) {
			continue_finder finder;
			sb->stmts[i]->accept(&finder);
			if (finder.has_break || finder.has_continue) {
				prior_is_eligible = false;
				break;
			}
		}

		if (prior_is_eligible) {
			stmt::Ptr then_stmt = if_body->then_stmt;
			stmt::Ptr else_stmt = if_body->else_stmt;
			if (isa<stmt_block>(else_stmt) && to<stmt_block>(else_stmt)->stmts.size() == 1) {
				if (isa<break_stmt>(to<stmt_block>(else_stmt)->stmts[0])) {
					new_while->cond = if_body->cond;
					// new_while->body =
					// std::make_shared<stmt_block>();
					new_while->body = then_stmt;
					// Push the new statements into the body
					bool implicit_changed = false;
					for (unsigned i = 0; i < sb->stmts.size() - 1; i++) {	
						// Statement needs to be pushed at the end of all continue blocks, 
						// implicit and explicit. However for the implicit ones, just push 
						// once at the end instead of in each of them
						to<stmt_block>(new_while->body)->stmts.push_back(sb->stmts[i]);
						for (auto p: new_while->continue_blocks) {
							auto cont_stmt = p->stmts.back();
							p->stmts.pop_back();
							p->stmts.push_back(sb->stmts[i]);
							p->stmts.push_back(cont_stmt);
						}
						stmts_before.push_back(sb->stmts[i]);
						implicit_changed = true;
					}
					if (implicit_changed) {
						new_while->implicit_continue_blocks.clear();
						new_while->implicit_continue_blocks.push_back(to<stmt_block>(new_while->body));
					}
					return;
				}
			}
			if (isa<stmt_block>(then_stmt) && to<stmt_block>(then_stmt)->stmts.size() == 1) {
				if (isa<break_stmt>(to<stmt_block>(then_stmt)->stmts[0])) {
					not_expr::Ptr new_cond = std::make_shared<not_expr>();
					new_cond->static_offset = if_body->cond->static_offset;
					new_cond->expr1 = if_body->cond;
					new_while->cond = new_cond;
					new_while->body = else_stmt;
					bool implicit_changed = false;
					for (unsigned i = 0; i < sb->stmts.size() - 1; i++) {
						to<stmt_block>(new_while->body)->stmts.push_back(sb->stmts[i]);
						for (auto p: new_while->continue_blocks) {
							auto cont_stmt = p->stmts.back();
							p->stmts.pop_back();
							p->stmts.push_back(sb->stmts[i]);
							p->stmts.push_back(cont_stmt);
						}
						stmts_before.push_back(sb->stmts[i]);
						implicit_changed = true;
					}
					if (implicit_changed) {
						new_while->implicit_continue_blocks.clear();
						new_while->implicit_continue_blocks.push_back(to<stmt_block>(new_while->body));
					}
					return;
				}
			}
		}
	}
	
	// Other pattern is if the loops first statement is a if condition that
	// breaks
	if (isa<if_stmt>(to<stmt_block>(new_while->body)->stmts[0])) {
		if_stmt::Ptr if_body = to<if_stmt>(to<stmt_block>(new_while->body)->stmts[0]);
		stmt::Ptr then_stmt = if_body->then_stmt;

		if (isa<stmt_block>(then_stmt) && to<stmt_block>(then_stmt)->stmts.size() == 1) {
			if (isa<break_stmt>(to<stmt_block>(then_stmt)->stmts[0])) {
				not_expr::Ptr new_cond = std::make_shared<not_expr>();
				new_cond->static_offset = if_body->cond->static_offset;
				new_cond->expr1 = if_body->cond;
				new_while->cond = new_cond;
				auto new_body = std::make_shared<stmt_block>();
				for (unsigned int i = 1; i < to<stmt_block>(new_while->body)->stmts.size(); i++) {
					new_body->stmts.push_back(to<stmt_block>(new_while->body)->stmts[i]);
				}
				new_while->body = new_body;
				return;
			}
		}
	}
}

void loop_finder::visit_label(label_stmt::Ptr a, stmt_block::Ptr parent) {

	// First separate out the stmts before the loop begin
	std::vector<stmt::Ptr> stmts_before;
	std::vector<stmt::Ptr> stmts_in_body;
	std::vector<stmt::Ptr> stmts_after_body;

	stmt::Ptr last_stmt = nullptr;

	for (auto stmt : parent->stmts) {
		last_jump_finder jump_finder;
		jump_finder.jump_label = a->label1;
		stmt->accept(&jump_finder);
		if (jump_finder.has_jump_to == true)
			last_stmt = stmt;
	}

	if (last_stmt == nullptr) {
		// This label was created but has no jump.
		// this currently happens when two statements have the same tag
		// For now we will just delete this label
		std::vector<stmt::Ptr> new_stmts;
		for (auto stmt : parent->stmts) {
			if (stmt == a)
				continue;
			new_stmts.push_back(stmt);
		}
		parent->stmts = new_stmts;
		return;
	}

	std::vector<stmt::Ptr>::iterator stmt;
	for (stmt = parent->stmts.begin(); stmt != parent->stmts.end(); stmt++) {
		if (*stmt == a)
			break;
		stmts_before.push_back(*stmt);
	}
	stmt++;
	for (; stmt != parent->stmts.end(); stmt++) {
		stmts_in_body.push_back(*stmt);
		if (*stmt == last_stmt)
			break;
	}
	stmt++;
	for (; stmt != parent->stmts.end(); stmt++) {
		stmts_after_body.push_back(*stmt);
	}
	parent->stmts.clear();

	while_stmt::Ptr new_while = std::make_shared<while_stmt>();
	new_while->cond = std::make_shared<int_const>();
	to<int_const>(new_while->cond)->value = 1;
	new_while->body = std::make_shared<stmt_block>();
	to<stmt_block>(new_while->body)->stmts = stmts_in_body;

	std::vector<stmt_block::Ptr> break_parents;
	std::vector<stmt_block::Ptr> implicit_continue_parents;

	// Clean up all loops in this body
	loop_finder finder;
	finder.ast = new_while->body;
	new_while->body->accept(&finder);

	ensure_back_has_goto(to<stmt_block>(new_while->body), a->label1, break_parents, implicit_continue_parents);
	// Record the explicit continues for later (for_loop_finder)
	new_while->implicit_continue_blocks = implicit_continue_parents;

	// For statements that are not at the end, find the gotos and insert explicit continues
	std::vector<stmt_block::Ptr> collects;
	insert_continues(to<stmt_block>(new_while->body), a->label1, collects);
	// Record the explicit continues for later
	new_while->continue_blocks = collects;

	// Gather blocks that have a break but are not at the end
	insert_breaks(to<stmt_block>(new_while->body), a->label1, break_parents);
	new_while->break_blocks = break_parents;

	std::vector<stmt::Ptr> trimmed;

	if (break_parents.size() > 0)
		trim_from_parents(break_parents, trimmed);

	// Now push a break to the end of every parent
	for (stmt_block::Ptr block : break_parents) {
		if (block->stmts.size() > 0 && isa<return_stmt>(block->stmts.back())) continue;
	
		break_stmt::Ptr new_break = std::make_shared<break_stmt>();
		block->stmts.push_back(new_break);
	}

	std::reverse(trimmed.begin(), trimmed.end());

	// stmts_before is passed by reference and can be updated
	merge_condition_with_loop(new_while, stmts_before);

	// Once we are happy with the loops, we have to make sure that this loop doesn't have any other jumps
	// If it does, we should pull them out. So outer loops can handle them
	outer_jump_finder outer_finder(loop_hook_counter);
	new_while->accept(&outer_finder);

	// For every control guard variable insert a initialization before the loop and the beginning of the loop

	std::vector<stmt::Ptr> new_body_stmts;
	std::vector<stmt::Ptr> guard_decl_stmts;
	std::vector<stmt::Ptr> guarded_jumps;
	for (auto guards : outer_finder.created_vars) {
		var::Ptr var1 = guards.first;

		auto var_expr1 = std::make_shared<var_expr>();
		var_expr1->var1 = var1;
		auto const_expr1 = std::make_shared<int_const>();
		const_expr1->value = 0;
		const_expr1->is_64bit = false;
		auto assign_expr1 = std::make_shared<assign_expr>();
		assign_expr1->var1 = var_expr1;
		assign_expr1->expr1 = const_expr1;

		auto expr_stmt1 = std::make_shared<expr_stmt>();
		expr_stmt1->expr1 = assign_expr1;

		new_body_stmts.push_back(expr_stmt1);

		auto var_decl1 = std::make_shared<decl_stmt>();
		var_decl1->decl_var = var1;
		var_decl1->init_expr = const_expr1;
		guard_decl_stmts.push_back(var_decl1);

		auto if_stmt1 = std::make_shared<if_stmt>();
		if_stmt1->else_stmt = std::make_shared<stmt_block>();
		auto stmt_block1 = std::make_shared<stmt_block>();
		if_stmt1->then_stmt = stmt_block1;
		stmt_block1->stmts.push_back(guards.second);

		auto var_expr2 = std::make_shared<var_expr>();
		var_expr2->var1 = var1;
		if_stmt1->cond = var_expr2;

		guarded_jumps.push_back(if_stmt1);
	}

	// Insert all the original statements
	for (auto stmt : to<stmt_block>(new_while->body)->stmts) {
		new_body_stmts.push_back(stmt);
	}
	to<stmt_block>(new_while->body)->stmts = new_body_stmts;

	// New while is ready to be inserted
	parent->stmts = stmts_before;
	// Insert the new guard decls we created
	for (auto stmt : guard_decl_stmts) {
		parent->stmts.push_back(stmt);
	}
	parent->stmts.push_back(new_while);
	// Insert the guaded jumps afer
	for (auto stmt : guarded_jumps) {
		parent->stmts.push_back(stmt);
	}
	for (auto stmt : trimmed) {
		parent->stmts.push_back(stmt);
	}
	for (auto stmt : stmts_after_body) {
		parent->stmts.push_back(stmt);
	}
}

void outer_jump_finder::visit(stmt_block::Ptr block) {
	// First visit all the statements normally
	block_visitor::visit(block);

	std::vector<stmt::Ptr> new_stmts;
	for (auto stmt : block->stmts) {
		if (isa<goto_stmt>(stmt)) {
			// We found a jump statement, this must escape this loop, otherwise it would have
			// been replaced with a continue
			// we should now create a new variable and assignment
			auto var1 = std::make_shared<var>();
			var1->var_name = "control_guard" + std::to_string(loop_hook_counter++);
			auto scalar_type1 = std::make_shared<scalar_type>();
			var1->var_type = scalar_type1;
			scalar_type1->scalar_type_id = scalar_type::INT_TYPE;
			auto var_expr1 = std::make_shared<var_expr>();
			var_expr1->var1 = var1;
			auto const_expr1 = std::make_shared<int_const>();
			const_expr1->value = 1;
			const_expr1->is_64bit = false;
			auto assign_expr1 = std::make_shared<assign_expr>();
			assign_expr1->var1 = var_expr1;
			assign_expr1->expr1 = const_expr1;

			auto expr_stmt1 = std::make_shared<expr_stmt>();
			expr_stmt1->expr1 = assign_expr1;

			new_stmts.push_back(expr_stmt1);

			created_vars.push_back(std::make_pair(var1, stmt));
		} else {
			new_stmts.push_back(stmt);
		}
	}
	block->stmts = new_stmts;
}

void last_jump_finder::visit(goto_stmt::Ptr a) {
	if (a->label1 == jump_label) {
		has_jump_to = true;
	}
}
} // namespace block
