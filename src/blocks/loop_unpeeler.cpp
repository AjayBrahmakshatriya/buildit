#include "blocks/loop_unpeeler.h"
#include "blocks/stmt.h"
namespace block {

/* peeled loop iterations are of the form - 

if (cond) {
   body;
   while (cond) {
      body;
   }
}

This should be collapsed into 

while (cond) {
   body;
}
*/
void loop_unpeeler::visit(if_stmt::Ptr is) {
	// recursively visit the if stmt before we do anything
	block_replacer::visit(is);
	
	node = is;	
	
	// Skip ifs that have non empty else
	if (!isa<stmt_block>(is->else_stmt)) return;
	if (to<stmt_block>(is->else_stmt)->stmts.size() != 0) return;

	// check if the last stmt in the then block is a while loop with the same condition
	if (!isa<stmt_block>(is->then_stmt)) return;
	stmt_block::Ptr then_block = to<stmt_block>(is->then_stmt);
	if (then_block->stmts.size() == 0) return;

	if (!isa<while_stmt>(then_block->stmts.back())) return;
	while_stmt::Ptr ws = to<while_stmt>(then_block->stmts.back());	
	
	if (!ws->cond->is_same(is->cond)) return;
		
	// The while loop should have exactly the same number of statements in the body 
	// besides the while loop
	
	if (!isa<stmt_block>(ws->body)) return;
	stmt_block::Ptr ws_body = to<stmt_block>(ws->body);
	
	if ((ws_body->stmts.size() + 1) != then_block->stmts.size()) return;

	// Match each statement
	for (unsigned i = 0; i < ws_body->stmts.size(); i++) {
		if (!ws_body->stmts[i]->is_same(then_block->stmts[i])) return;
	}

	// everything looks good, replace with the while
	
	node = ws;
	return;
}


}

