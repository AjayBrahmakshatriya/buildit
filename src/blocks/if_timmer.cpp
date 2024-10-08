#include "blocks/if_trimmer.h"
#include "blocks/stmt.h"

namespace block {

void if_trimmer::visit(stmt_block::Ptr sb) {
	block_visitor::visit(sb);

	std::vector<stmt::Ptr> new_stmts;
	for (auto stmt: sb->stmts) {
		new_stmts.push_back(stmt);
		if (!isa<if_stmt>(stmt)) continue;
		auto is = to<if_stmt>(stmt);
		if (!isa<stmt_block>(is->then_stmt)) continue;
		if (!isa<stmt_block>(is->else_stmt)) continue;
		stmt_block::Ptr tb = to<stmt_block>(is->then_stmt);
		stmt_block::Ptr eb = to<stmt_block>(is->else_stmt);
		std::vector<stmt::Ptr> temp_stmts;
		while(tb->stmts.size() != 0 && eb->stmts.size() != 0) {
			auto s1 = tb->stmts.back();
			auto s2 = eb->stmts.back();
			if (!s1->is_same(s2)) break;
			tb->stmts.pop_back();
			eb->stmts.pop_back();
			temp_stmts.push_back(s1);
		}
		while (temp_stmts.size()) {
			new_stmts.push_back(temp_stmts.back());
			temp_stmts.pop_back();
		}
	}
	
	sb->stmts = new_stmts;
}


}
