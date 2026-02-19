#include "blocks/if_switcher.h"

namespace block {


class continue_counter: public block_visitor {
public:
	int count = 0;
	using block_visitor::visit;
	virtual void visit(continue_stmt::Ptr) {
		count++;
	}

	// Do not recurse for loops
	virtual void visit(while_stmt::Ptr) {
		return;
	}
	virtual void visit(for_stmt::Ptr) {
		return;
	}
};



void if_switcher::visit(if_stmt::Ptr ifs) {
	if (isa<stmt_block>(ifs->then_stmt)) {
		auto then_block = to<stmt_block>(ifs->then_stmt);
		auto else_block = to<stmt_block>(ifs->else_stmt);
		if (then_block->stmts.size() == 0 && else_block->stmts.size() != 0) {
			ifs->then_stmt = ifs->else_stmt;
			ifs->else_stmt = then_block;
			auto ne = std::make_shared<not_expr>();
			ne->expr1 = ifs->cond;
			ifs->cond = ne;
		}
		
	}
	block_replacer::visit(ifs);	
	node = ifs;
}




} // namespace block
