#include "builder/builder_context.h"
#include "blocks/for_loop_finder.h"
#include "blocks/if_switcher.h"
#include "blocks/if_flattener.h"
#include "blocks/label_inserter.h"
#include "blocks/loop_finder.h"
#include "blocks/loop_roll.h"
#include "blocks/rce.h"
#include "blocks/sub_expr_cleanup.h"
#include "blocks/generic_checker.h"
#include "blocks/var_namer.h"
#include "builder/dyn_var.h"
#include "builder/exceptions.h"
#include "util/tracer.h"
#include <algorithm>

namespace builder {

static std::vector<block::stmt::Ptr> trim_common_from_back(block::stmt::Ptr ast1, block::stmt::Ptr ast2) {
	std::vector<block::stmt::Ptr> trimmed_stmts;
	std::vector<block::stmt::Ptr> &ast1_stmts = block::to<block::stmt_block>(ast1)->stmts;
	std::vector<block::stmt::Ptr> &ast2_stmts = block::to<block::stmt_block>(ast2)->stmts;

	while (ast1_stmts.size() > 0 && ast2_stmts.size() > 0 && ast1_stmts.back()->is_same(ast2_stmts.back())) {
		block::stmt::Ptr trimmed_stmt = ast1_stmts.back();
		ast1_stmts.pop_back();
		ast2_stmts.pop_back();
		trimmed_stmts.push_back(trimmed_stmt);
	}

	// If the only statement trimmed is a return, add it back to the branches
	// that way the next special case can keep the generated code clean
	if (trimmed_stmts.size() == 1 && block::isa<block::return_stmt>(trimmed_stmts.back())) {
		ast1_stmts.push_back(trimmed_stmts.back());
		ast2_stmts.push_back(trimmed_stmts.back());
		trimmed_stmts.pop_back();	
	}

	std::reverse(trimmed_stmts.begin(), trimmed_stmts.end());
	return trimmed_stmts;
}

void builder_context::extract_function_ast_impl(invocation_state* i_state) {

	last_num_runs = 0;

#ifndef ENABLE_D2X
	if (enable_d2x)
		assert(false && "D2X support cannot be enabled without the ENABLE_D2X build option");
#endif
	block::stmt::Ptr ast = nullptr;
	// Repeat till ND vars are happy

	int nd_count = 0;

	while (1) {
		try {
			nd_count++;
			// Allocate one execution_state for this ND run
			execution_state e_state (i_state);
			// Allocate one run_state, rest will be allocated by the recursive calls
			run_state r_state (&e_state, i_state);
			ast = extract_ast_from_run(&r_state);
		} catch (NonDeterministicFailureException &e) {
			// Before we continue, reset the area, so static 
			// vars allocated in the arena don't spill to the next execution	
			// don't use get_invocation_state because r_state is not live

			// but destructor will try to access r_state, so clear it first
			run_state::current_run_state = nullptr;	
			i_state->get_arena()->reset_arena();
			continue;
		}
		break;
	}
	std::cerr << "Final ND runs = " << nd_count << std::endl;

	// Before making any changes, untangle the whole AST
	ast = clone(ast);
	
	// Make sure any generics haven't been left 
	// unspecialized
	block::generic_null_checker checker;
	ast->accept(&checker);

	block::var_namer::name_vars(ast);

	block::label_collector collector;
	ast->accept(&collector);

	block::label_creator creator;
	creator.collected_labels = collector.collected_labels;
	ast->accept(&creator);

	block::label_inserter inserter;
	inserter.backup_offset_to_label = creator.offset_to_label;
	inserter.feature_unstructured = feature_unstructured;
	ast->accept(&inserter);

	// At this point it is safe to remove statements that are
	// marked for deletion
	block::sub_expr_cleanup cleaner;
	ast->accept(&cleaner);

	block::recursive_merger merger;
	ast->accept(&merger);

	block::if_flattener flattener(false);
	ast->accept(&flattener);
	
	// Run a pre-loop extraction RCE to help with loop extraction
	if (run_rce) {
		block::eliminate_redundant_vars(ast);
	}
	
	if (!feature_unstructured ) {

		block::basic_block::cfg_block BBs = generate_basic_blocks(block::to<block::stmt_block>(ast));
		
		block::loop_finder finder;
		finder.ast = ast;
		ast->accept(&finder);

		block::if_switcher switcher;
		ast->accept(&switcher);	
	
		block::fix_loop_inversion inv_fixer;
		ast->accept(&inv_fixer);

		block::for_loop_finder for_finder;
		for_finder.ast = ast;
		ast->accept(&for_finder);

		// Post loop finder flattener doesn't ignore
		// double sided jumps
		block::if_flattener flattener(false);
		ast->accept(&flattener);

		ast->accept(&switcher);	
	}
	// Run RCE after loop finder
	// since RCE does rely on loops being detected
	// If labels are still kept around, RCE cannot be as aggressive 
	// since it has to consider the worst case
	if (run_rce) {
		block::eliminate_redundant_vars(ast);
	}

	block::loop_roll_finder loop_roll_finder;
	ast->accept(&loop_roll_finder);


	i_state->generated_func_decl->body = ast;	
}
block::stmt::Ptr builder_context::extract_ast_from_run(run_state* r_state) {
	// Update stats
	last_num_runs++;	

	r_state->current_stmt_block = std::make_shared<block::stmt_block>();
	block::stmt_block::Ptr ast = r_state->current_stmt_block;

	// A new run is starting, clear the parent stack
	// for identifying nested members. This is because a run can end mid construction
	if (parents_stack != nullptr) {
		parents_stack->clear();
	}

	block::stmt_block::Ptr ret_ast;

	std::vector<bool> bool_vector_copy = r_state->bool_vector;

	try {
		run_state::current_run_state = r_state;
		// function();
		lambda_wrapper(r_state->i_state->invocation_function);
		r_state->commit_uncommitted();
		// If the run state has had an nd var fault that has not been thrown, 
		// throw now
		if (r_state->needs_nd_rerun) {
			throw NonDeterministicFailureException();	
		}
		ret_ast = ast;
		get_invocation_state()->get_arena()->reset_arena();
		run_state::current_run_state = nullptr;
	} catch (OutOfBoolsException &e) {

		// If the run state has had an nd var fault that has not been thrown, 
		// throw now
		if (r_state->needs_nd_rerun) {
			throw NonDeterministicFailureException();	
		}

		// Reset dyn_var arena before starting new runs
		get_invocation_state()->get_arena()->reset_arena();

		run_state::current_run_state = nullptr;

		block::expr_stmt::Ptr last_stmt = block::to<block::expr_stmt>(r_state->current_stmt_block->stmts.back());
		r_state->current_stmt_block->stmts.pop_back();

		block::expr::Ptr cond_expr = last_stmt->expr1;
	
		// Establish two run_states
		run_state true_r_state(r_state->e_state, r_state->i_state);
		// Only copy over the expr_sequence since it is part of the r_state that 
		// just terminated
		true_r_state.cached_expr_sequence = r_state->cached_expr_sequence;
		true_r_state.bool_vector.push_back(true);
		true_r_state.visited_offsets = r_state->visited_offsets;
		true_r_state.tag_deduplication_map = r_state->tag_deduplication_map;
		std::copy(bool_vector_copy.begin(), bool_vector_copy.end(), std::back_inserter(true_r_state.bool_vector));

		// Establish two run_states
		run_state false_r_state(r_state->e_state, r_state->i_state);
		false_r_state.visited_offsets = r_state->visited_offsets;
		false_r_state.tag_deduplication_map = r_state->tag_deduplication_map;
		// Only copy over the expr_sequence since it is part of the r_state that 
		// just terminated
		false_r_state.cached_expr_sequence = r_state->cached_expr_sequence;
		false_r_state.bool_vector = true_r_state.bool_vector;
		false_r_state.bool_vector[0] = false;

		block::stmt_block::Ptr true_ast = block::to<block::stmt_block>(extract_ast_from_run(&true_r_state));
		block::stmt_block::Ptr false_ast = block::to<block::stmt_block>(extract_ast_from_run(&false_r_state));

		std::vector<block::stmt::Ptr> trimmed_stmts = trim_common_from_back(true_ast, false_ast);
		r_state->erase_tag(e.static_offset);

		block::if_stmt::Ptr new_if_stmt = std::make_shared<block::if_stmt>();
		new_if_stmt->static_offset = e.static_offset;
		new_if_stmt->cond = cond_expr;
		new_if_stmt->then_stmt = true_ast;
		new_if_stmt->else_stmt = false_ast;
		new_if_stmt->annotation = last_stmt->annotation;

		r_state->add_stmt_to_current_block(new_if_stmt, false);

		std::copy(trimmed_stmts.begin(), trimmed_stmts.end(), std::back_inserter(r_state->current_stmt_block->stmts));

		ret_ast = ast;
	} catch (LoopBackException &e) {
		// If the run state has had an nd var fault that has not been thrown, 
		// throw now
		if (r_state->needs_nd_rerun) {
			throw NonDeterministicFailureException();	
		}
		get_invocation_state()->get_arena()->reset_arena();
		run_state::current_run_state = nullptr;
		block::goto_stmt::Ptr goto_stmt = std::make_shared<block::goto_stmt>();
		goto_stmt->static_offset.clear();
		goto_stmt->temporary_label_number = e.static_offset;

		r_state->add_stmt_to_current_block(goto_stmt, false);
		ret_ast = ast;
	} catch (MemoizationException &e) {
		// If the run state has had an nd var fault that has not been thrown, 
		// throw now
		if (r_state->needs_nd_rerun) {
			throw NonDeterministicFailureException();	
		}
		get_invocation_state()->get_arena()->reset_arena();
		run_state::current_run_state = nullptr;
		if (feature_unstructured) {
			// Instead of copying statements to the current block, we will just insert a goto
			block::goto_stmt::Ptr goto_stmt = std::make_shared<block::goto_stmt>();
			goto_stmt->static_offset.clear();
			goto_stmt->temporary_label_number = e.static_offset;
			r_state->add_stmt_to_current_block(goto_stmt, false);
		} else {
			for (unsigned int i = e.child_id; i < e.parent->stmts.size(); i++) {
				if (block::isa<block::goto_stmt>(e.parent->stmts[i])) {
					block::goto_stmt::Ptr goto_stmt = std::make_shared<block::goto_stmt>();
					goto_stmt->static_offset.clear();
					goto_stmt->temporary_label_number = block::to<block::goto_stmt>(e.parent->stmts[i])->temporary_label_number;
					r_state->add_stmt_to_current_block(goto_stmt, false);
				}
				else {
					r_state->add_stmt_to_current_block(e.parent->stmts[i], false);
				}
			}
		}
		ret_ast = ast;
	} 


	run_state::current_run_state = nullptr;

	// Update the memoized table with the stmt block we just created
	for (unsigned int i = 0; i < r_state->current_stmt_block->stmts.size(); i++) {
		block::stmt::Ptr s = r_state->current_stmt_block->stmts[i];
		// If any of the statements are if conditions, remove the
		// internal statements from the table
		// This is required because of the way we do memoization.
		// We just store a pointer to the stmt_block that has the statement.
		// In case of if stmt, the then and else branches don't have all the required
		// statements. There might be statements AFTER the if which will not be
		// included in the memoization result.
		// But, if we are using feature_unstructured, we don't really care about
		// the statements, just the existence of the statement. So we will reset
		// the memoization table to point to the new blocks around
		if (block::isa<block::if_stmt>(s)) {
			block::if_stmt::Ptr if1 = block::to<block::if_stmt>(s);
			assert(block::isa<block::stmt_block>(if1->then_stmt));
			assert(block::isa<block::stmt_block>(if1->else_stmt));
			for (auto &stmt : block::to<block::stmt_block>(if1->then_stmt)->stmts) {
				auto it = r_state->e_state->memoized_tags.find(stmt->static_offset);
				if (it != r_state->e_state->memoized_tags.end())
					r_state->e_state->memoized_tags.erase(it);

				if (feature_unstructured) {
					auto pblock = block::to<block::stmt_block>(if1->then_stmt);
					r_state->e_state->memoized_tags[stmt->static_offset] = pblock;
				}
			}
			for (auto &stmt : block::to<block::stmt_block>(if1->else_stmt)->stmts) {
				auto it = r_state->e_state->memoized_tags.find(stmt->static_offset);
				if (it != r_state->e_state->memoized_tags.end())
					r_state->e_state->memoized_tags.erase(it);
				if (feature_unstructured) {
					auto pblock = block::to<block::stmt_block>(if1->else_stmt);
					r_state->e_state->memoized_tags[stmt->static_offset] = pblock;
				}
			}
		}
		r_state->e_state->memoized_tags[s->static_offset] = r_state->current_stmt_block;
	}

	ast = r_state->current_stmt_block = nullptr;
	return ret_ast;
}

} // namespace builder
