#ifndef LOOP_FINDER_H
#define LOOP_FINDER_H
#include "blocks/block_visitor.h"
#include "blocks/stmt.h"

namespace block {
class loop_finder: public block_visitor {
public:
	stmt::Ptr ast;
	virtual void visit(label_stmt::Ptr);
	
};
class last_jump_finder: public block_visitor {
public:
	bool has_jump_to = false;
	label::Ptr jump_label;
	
	virtual void visit(goto_stmt::Ptr);	
};

}
#endif
