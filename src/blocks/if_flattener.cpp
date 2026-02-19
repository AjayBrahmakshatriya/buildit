#include "blocks/if_flattener.h"
#include <algorithm>
namespace block {

static bool is_terminal(stmt::Ptr a) {
	return isa<return_stmt>(a) || isa<goto_stmt>(a) || isa<continue_stmt>(a) || isa<break_stmt>(a);
}


void if_flattener::visit(stmt_block::Ptr b) {
	// first visit recursively
	block_visitor::visit(b);

	std::vector<stmt::Ptr> new_stmts;

	for (auto s: b->stmts) {
		new_stmts.push_back(s);
		if (isa<if_stmt>(s)) {
			std::vector<stmt::Ptr> &ast1_stmts = to<stmt_block>(to<if_stmt>(s)->then_stmt)->stmts;
			std::vector<stmt::Ptr> &ast2_stmts = to<stmt_block>(to<if_stmt>(s)->else_stmt)->stmts;
			if (ast1_stmts.size() != 0 && ast2_stmts.size() != 0) {
				if (ignore_double_sided_jumps && is_terminal(ast1_stmts.back()) && is_terminal(ast2_stmts.back())) {
					
				} else if (is_terminal(ast1_stmts.back())) {
					for (auto sx: ast2_stmts) {
						new_stmts.push_back(sx);
					}
					ast2_stmts.clear();
				} else if (is_terminal(ast2_stmts.back())) {
					for (auto sx: ast1_stmts) {
						new_stmts.push_back(sx);
					}
					ast1_stmts.clear();
				}
			}
		}
	}
	b->stmts = new_stmts;
}

static std::vector<stmt::Ptr> trim_common(std::vector<stmt::Ptr> &ast1_stmts, std::vector<stmt::Ptr> &ast2_stmts) {
	std::vector<stmt::Ptr> trimmed_stmts;
	while (ast1_stmts.size() > 0 && ast2_stmts.size() > 0 && ast1_stmts.back()->is_same(ast2_stmts.back())) {
		stmt::Ptr trimmed_stmt = ast1_stmts.back();
		ast1_stmts.pop_back();
		ast2_stmts.pop_back();
		trimmed_stmts.push_back(trimmed_stmt);
	}

	// If the only statement trimmed is a return, add it back to the branches
	// that way the next special case can keep the generated code clean
	if (trimmed_stmts.size() == 1 && isa<return_stmt>(trimmed_stmts.back())) {
		ast1_stmts.push_back(trimmed_stmts.back());
		ast2_stmts.push_back(trimmed_stmts.back());
		trimmed_stmts.pop_back();	
	}

	if (ast1_stmts.size() > 0 && ast2_stmts.size() > 0) {
		if (isa<if_stmt>(ast1_stmts.back())) {	
			auto chi = to<if_stmt>(ast1_stmts.back());
			std::vector<stmt::Ptr> &ch1 = to<stmt_block>(to<if_stmt>(chi)->then_stmt)->stmts;
			std::vector<stmt::Ptr> &ch2 = to<stmt_block>(to<if_stmt>(chi)->else_stmt)->stmts;
			std::vector<stmt::Ptr> trimmed;
			if (ch1.size() > 0 && is_terminal(ch1.back())) {
				trimmed = trim_common(ch2, ast2_stmts);
			}					
			if (trimmed.size() == 0 && ch2.size() > 0 && is_terminal(ch2.back())) {
				trimmed = trim_common(ch1, ast2_stmts);
			}
			std::reverse(trimmed.begin(), trimmed.end());	
			for (auto sx: trimmed) {
				trimmed_stmts.push_back(sx);
			}
		} else if (isa<if_stmt>(ast2_stmts.back())) {
			auto chi = to<if_stmt>(ast2_stmts.back());
			std::vector<stmt::Ptr> &ch1 = to<stmt_block>(to<if_stmt>(chi)->then_stmt)->stmts;
			std::vector<stmt::Ptr> &ch2 = to<stmt_block>(to<if_stmt>(chi)->else_stmt)->stmts;
			std::vector<stmt::Ptr> trimmed;
			if (ch1.size() > 0 && is_terminal(ch1.back())) {
				trimmed = trim_common(ast1_stmts, ch2);
			}					
			if (trimmed.size() == 0 && ch2.size() > 0 && is_terminal(ch2.back())) {
				trimmed = trim_common(ast1_stmts, ch1);
			}
			std::reverse(trimmed.begin(), trimmed.end());	
			for (auto sx: trimmed) {
				trimmed_stmts.push_back(sx);
			}
		}
	}

	std::reverse(trimmed_stmts.begin(), trimmed_stmts.end());
	return trimmed_stmts;
}

void recursive_merger::visit(stmt_block::Ptr b) {
	// First visit recursively
	block_visitor::visit(b);

	std::vector<stmt::Ptr> new_stmts;
	for (auto s: b->stmts) {
		new_stmts.push_back(s);
		if (isa<if_stmt>(s)) {
			// We have an if who's tails cannot be merged directly
			std::vector<stmt::Ptr> &ast1_stmts = to<stmt_block>(to<if_stmt>(s)->then_stmt)->stmts;
			std::vector<stmt::Ptr> &ast2_stmts = to<stmt_block>(to<if_stmt>(s)->else_stmt)->stmts;
			auto trimmed = trim_common(ast1_stmts, ast2_stmts);
			for (auto sx: trimmed) {
				new_stmts.push_back(sx);
			}	
		}
	}
	b->stmts = new_stmts;
}
}
