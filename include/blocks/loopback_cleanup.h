#ifndef LOOPBACK_CLEANUP_H
#define LOOPBACK_CLEANUP_H

#include "blocks/block_visitor.h"

namespace block {

class loopback_cleanup: public block_visitor {
public:
	using block_visitor::visit;

	virtual void visit(std::shared_ptr<stmt_block>) override;

};


}


#endif

