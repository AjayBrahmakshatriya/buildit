#include "blocks/var.h"
#include "util/printer.h"

namespace block{
void var::dump(std::ostream &oss, int indent) {
	printer::indent(oss, indent);
	oss << "VAR (" << var_name << ")" << std::endl;	

#ifdef DEBUG_VARS	
	printer::indent(oss, indent);
	oss << static_offset.stringify() << std::endl;
#endif
}
void type::dump(std::ostream &oss, int indent) {
}
void scalar_type::dump(std::ostream &oss, int indent) {
	printer::indent(oss, indent);
	oss << "SCALAR_TYPE (";
	if (scalar_type_id == INT_TYPE)
		oss << "INT";
	if (scalar_type_id == CHAR_TYPE)
		oss << "CHAR";
	else if (scalar_type_id == VOID_TYPE)
		oss << "VOID";
	oss << ")" << std::endl;
}
void pointer_type::dump(std::ostream &oss, int indent) {
	printer::indent(oss, indent);
	oss << "POINTER_TYPE" << std::endl;
	pointee_type->dump(oss, indent+1);
}
void function_type::dump(std::ostream &oss, int indent) {
	printer::indent(oss, indent);
	oss << "FUNCITON_TYPE" << std::endl;
	return_type->dump(oss, indent+1);
	for (unsigned int i = 0; i < arg_types.size(); i++) 
		arg_types[i]->dump(oss, indent+1);
}
void array_type::dump(std::ostream &oss, int indent) {
	printer::indent(oss, indent);
	oss << "ARRAY_TYPE" << std::endl;
	element_type->dump(oss, indent+1);
	printer::indent(oss, indent+1);
	oss << size << std::endl;
	
}
}
