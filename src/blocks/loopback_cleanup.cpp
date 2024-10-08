#include "blocks/loopback_cleanup.h"
#include "blocks/stmt.h"

namespace block {

/* This pass looks for the patterns of the type - 

if (cond) {
label_x:
  ...
  if (cond) {
     goto label_x;
  }
}
 
and simplifies them into 

label_y:
if (cond) {
label_x:
    ...
    goto label_y;
}

This works for all recursive cases as long the inner if is the last thing to happen in the outer if
*/

static unsigned loopback_label_counter = 0;

static void fixup_recursive(stmt_block::Ptr sb, label::Ptr lb, label::Ptr &new_label, expr::Ptr cond) {
	if (sb->stmts.size() == 0) return;

	// First check if this fits our pattern
	if (isa<if_stmt>(sb->stmts.back())) {
		auto is = to<if_stmt>(sb->stmts.back());
		if (is->cond->is_same(cond)) {
			if (isa<stmt_block>(is->else_stmt) && to<stmt_block>(is->else_stmt)->stmts.size() == 0) {
				if (isa<stmt_block>(is->then_stmt) && to<stmt_block>(is->then_stmt)->stmts.size() == 1) {
					auto ls = to<stmt_block>(is->then_stmt)->stmts[0];
					if (isa<goto_stmt>(ls) && to<goto_stmt>(ls)->label1 == lb) {
						// This is a match
						sb->stmts.pop_back();
						sb->stmts.push_back(ls);
						// Now patch the label
						if (new_label == nullptr) {
							new_label = std::make_shared<label>();
							new_label->label_name = "fixup_label" + std::to_string(loopback_label_counter);
							loopback_label_counter++;
						}
						to<goto_stmt>(ls)->label1 = new_label;
						return;
					}
				}
			}
		}
		if (isa<stmt_block>(is->then_stmt)) {
			fixup_recursive(to<stmt_block>(is->then_stmt), lb, new_label, cond);
		}
		if (isa<stmt_block>(is->else_stmt)) {
			fixup_recursive(to<stmt_block>(is->else_stmt), lb, new_label, cond);
		}
	}

}



void loopback_cleanup::visit(stmt_block::Ptr sb) {
	block_visitor::visit(sb);	

	std::vector<stmt::Ptr> new_stmts;
	for (auto stmt: sb->stmts) {
		if (isa<if_stmt>(stmt)) {
			auto is = to<if_stmt>(stmt);
			// The outer if shouldn't have an else
			if (isa<stmt_block>(is->else_stmt) && to<stmt_block>(is->else_stmt)->stmts.size() == 0) {
				if (isa<stmt_block>(is->then_stmt) && to<stmt_block>(is->then_stmt)->stmts.size() > 0) {
					// First statement should be a label
					stmt_block::Ptr tb = to<stmt_block>(is->then_stmt);
					auto fs = tb->stmts[0];
					if (isa<label_stmt>(fs)) {
						label::Ptr new_label = nullptr;
						fixup_recursive(tb, to<label_stmt>(fs)->label1, new_label, is->cond);	
						if (new_label != nullptr) {
							auto nl = std::make_shared<label_stmt>();
							nl->label1 = new_label;	
							new_stmts.push_back(nl);	
						}
					}
				}
			}
		}
		new_stmts.push_back(stmt);
	}
	sb->stmts = new_stmts;
}

}
