#include "builder/dyn_var.h"
#include "blocks/var.h"
#include "builder/generics.h"
namespace builder {

static std::vector<std::tuple<type, type, type>> *binary_op_res_type_table = nullptr;
static std::vector<std::pair<type, type>> *promoted_type_table = nullptr;

template <typename...Ts>
void ignore(Ts...args) {}

template <typename T1, typename T2>
static int insert_type_pair(void) {
	binary_op_res_type_table->push_back(
		std::make_tuple(
			create_type<T1>(), 
			create_type<T2>(), 
			create_type<decltype(std::declval<T1>() + std::declval<T2>())>()
		)
	);
	return 0;
}

template <typename T1>
static int fill_promotion(void) {
	promoted_type_table->push_back(std::make_pair(create_type<T1>(), create_type<decltype(+std::declval<T1>())>()));
	return 0;
}

template <typename T1, typename...Ts>
static int fill_for_type(void) {
	ignore(insert_type_pair<T1, Ts>()...);
	return 0;
}

template <typename...Ts>
static void fill_all_pairs(void) {
	ignore(fill_for_type<Ts, Ts...>()...);
}

template <typename...Ts>
static void fill_all_promotions(void) {
	ignore(fill_promotion<Ts>()...);
}

static void populate_op_res_type_table(void) {
	binary_op_res_type_table = new std::vector<std::tuple<type, type, type>>();
	promoted_type_table = new std::vector<std::pair<type, type>>();	
	fill_all_pairs<short int, unsigned short int, int, unsigned int, long int, 
			unsigned long int, long long int, unsigned long long int, 
			char, unsigned char, float, double, long double, bool,
			signed char>();	
	fill_all_promotions<short int, unsigned short int, int, unsigned int, long int, 
			unsigned long int, long long int, unsigned long long int, 
			char, unsigned char, float, double, long double, bool,
			signed char>();	
}

block::type::Ptr compute_binary_op_type(block::type::Ptr _t1, block::type::Ptr _t2, std::string op) {
	auto t1 = type(_t1);
	auto t2 = type(_t2);
	if (t1.enclosed_type == nullptr || t2.enclosed_type == nullptr) {
		assert(false && "Binary operation on generic variable without type");
		return nullptr;
	}

	if (op == "<" || op == ">" || op == "<=" || op == ">=" || op == "==" || op == "!=")
		return create_type<bool>().enclosed_type;

	if (op == "&&" || op == "||")
		return create_type<bool>().enclosed_type;

	if (is_reference(t1)) t1 = remove_reference(t1);
	if (is_reference(t2)) t2 = remove_reference(t2);

	if (is_array(t1)) t1 = pointer_of(remove_array(t1));
	if (is_array(t2)) t2 = pointer_of(remove_array(t2));

	if (is_pointer(t1) && is_pointer(t2)) {
		// Better be substraction
		if (op == "-") {
			return create_type<ptrdiff_t>().enclosed_type;
		}
		assert(false && "Operation cannot be performed on pointers");
		return nullptr;
	}
	// If one is pointer, the resultant type is the same pointer
	if (is_pointer(t1)) {
		if (op == "+" || op == "-") 
			return t1.enclosed_type;
		assert(false && "Operation cannot be performed on pointers");
		return nullptr;
	}
	if (is_pointer(t2)) {
		if (op == "+" || op == "-") 
			return t2.enclosed_type;
		assert(false && "Operation cannot be performed on pointers");
		return nullptr;
	}

	if (binary_op_res_type_table == nullptr) {
		populate_op_res_type_table();
	}	
	// Both types at this must be scalars or some custom types
	// Check the type table and decide
	if (op == ">>" || op == "<<") {
		for (auto p: *promoted_type_table) {
			if (p.first == t1) return p.second.enclosed_type;
		}
		assert(false && "Cannot compute the resultant type for binary op");
		return nullptr;
	}

	for (auto p: *binary_op_res_type_table) {
		if (std::get<0>(p) == t1 && std::get<1>(p) == t2) 
			return std::get<2>(p).enclosed_type;
	}
	assert(false && "Cannot compute the resultant type for binary op");
	return nullptr;
}

block::type::Ptr compute_unary_op_type(block::type::Ptr _t, std::string op) {
	auto t = type(_t);
	if (t.enclosed_type == nullptr) {
		assert(false && "Unary operation on generic variable without type");
		return nullptr;
	}	
	if (is_reference(t)) t = remove_reference(t);	

	if (op == "&") return pointer_of(t).enclosed_type;
	
	if (op == "*") {
		if (!is_pointer(t)) {
			assert(false && "Cannot dereference type that is not a pointer");
			return nullptr;
		}
		return remove_pointer(t).enclosed_type;
	}

	if (op == "!") return create_type<bool>().enclosed_type;

	// At this point it must be +, -, ~
	// which all just return the promoted type
	if (promoted_type_table == nullptr) {
		populate_op_res_type_table();
	}
	
	for (auto p: *promoted_type_table) {
		if (p.first == t) return p.second.enclosed_type;
	}
	
	assert(false && "Cannot compute the resultant type for unary op");		
	return nullptr;
}
}
