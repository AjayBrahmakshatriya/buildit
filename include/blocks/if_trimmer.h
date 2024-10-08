#ifndef IF_TRIMMER_H
#define IF_TRIMMER_H

#include "blocks/block_visitor.h"

namespace block {

// This is a cleanup pass related to loop_unpeel
// After unpeeling, more "trim from back" opportunities are uncovered
// This pass also generally can be run later for cleanups

class if_trimmer: public block_visitor {
public:
	using block_visitor::visit;
	virtual void visit(std::shared_ptr<stmt_block>) override;

};

}


#endif
