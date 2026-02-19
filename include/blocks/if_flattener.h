#ifndef BLOCKS_IF_FLATENNER_H
#define BLOCKS_IF_FLATENNER_H

#include "blocks/block_visitor.h"
#include "blocks/stmt.h"

namespace block {

class if_flattener: public block_visitor {
public:
	using block_visitor::visit;
	bool ignore_double_sided_jumps;

	if_flattener(bool ig): ignore_double_sided_jumps(ig) {}

	virtual void visit(stmt_block::Ptr);
	//virtual void visit(while_stmt::Ptr);
};

class recursive_merger: public block_visitor {
public:
	using block_visitor::visit;
	virtual void visit (stmt_block::Ptr);

};

}

#endif
