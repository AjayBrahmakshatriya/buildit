#ifndef LOOP_UNPEELER_H
#define LOOP_UNPEELER_H

#include "blocks/block_replacer.h"
namespace block {

// GCC tends to peel loop iterations 
// which messes up the fuzzer
// This is an attempt to undo that so we can have syntactic equivalence
class loop_unpeeler: public block_replacer {
public:
	using block_replacer::visit;
	virtual void visit(std::shared_ptr<if_stmt>) override;
};
}
#endif
