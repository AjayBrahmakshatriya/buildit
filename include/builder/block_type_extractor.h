#ifndef TYPE_EXTRACTOR_H
#define TYPE_EXTRACTOR_H


#include "builder/forward_declarations.h"
#include "builder/builder_context.h"
#include "builder/generics.h"
#include "builder/dyn_var.h"
#include <algorithm>

namespace builder {

struct custom_type_base;

template <typename T>
struct external_type_namer;



extern int type_naming_counter;

template <typename T, typename V=void>
struct type_namer {
	// Use a pointer to a string instead of 
	// a string because it is possible get_type_name is called
	// from global constructors before obtained_name is initialized
	static std::string *obtained_name;
	static std::string get_type_name() {
		if (obtained_name == nullptr) {
			obtained_name = new std::string();
			*obtained_name = "custom_struct" + std::to_string(type_naming_counter++);
		}
		return *obtained_name;
	}
};

template <typename T, typename V>
std::string *type_namer<T, V>::obtained_name = nullptr;

/* Specializations that check for typenames provided either as a 
member or a external_namer specialization */

template <typename T, typename V=void>
struct has_type_name: public std::false_type {};
template <typename T>
struct has_type_name<T, typename check_valid_type<decltype(T::type_name)>::type>: public std::true_type {};

template <typename T>
struct type_namer<T, typename std::enable_if<has_type_name<external_type_namer<T>>::value>::type> {
	static std::string get_type_name() {
		return external_type_namer<T>::type_name;
	}	
};

template <typename T>
struct type_namer<T, typename std::enable_if<!has_type_name<external_type_namer<T>>::value && 
	has_type_name<T>::value>::type> {
	static std::string get_type_name() {
		return T::type_name;
	}	
};

template <typename T, typename V=void>
struct type_template {
	static std::vector<block::type::Ptr> get_templates() {
		return {};
	}
};

template <typename T>
struct type_template<T, typename check_valid_type<decltype(T::get_template_arg_types)>::type> {
	static std::vector<block::type::Ptr> get_templates() {
		return T::get_template_arg_types();
	}
};

template <typename T>
struct type_template<T, typename check_valid_type<decltype(external_type_namer<T>::get_template_arg_types)>::type> {
	static std::vector<block::type::Ptr> get_templates() {
		return external_type_namer<T>::get_template_arg_types();
	}
};

template <typename T>
block::struct_decl::Ptr extract_struct_decl(void) {
	auto save = options::track_members;
	options::track_members = true;
	dyn_var<T> v = with_name("_");
	options::track_members = save;

	// Construct a struct decl
	auto sd = std::make_shared<block::struct_decl>();
	auto var_type = dyn_var<T>::create_block_type();
	assert(block::isa<block::named_type>(var_type) && "Cannot create struct declarations for un-named types");
	assert(block::to<block::named_type>(var_type)->template_args.size() == 0 &&
	       "Cannot yet, generate decls for types with template args");

	sd->struct_name = block::to<block::named_type>(var_type)->type_name;
	for (auto member: v.members) {
		auto decl = std::make_shared<block::decl_stmt>();
		decl->decl_var = member->block_var;
		decl->init_expr = nullptr;
		sd->members.push_back(decl);
	}
	return sd;
}


// The main definition of the type extractor classes
template <typename T>
class type_extractor {
public:
	// This implementation is currenty only used
	// by custom types which are derived from custom_type_base
	static block::type::Ptr extract_type(void) {
		block::named_type::Ptr type = std::make_shared<block::named_type>();
		type->type_name = type_namer<T>::get_type_name();
		type->template_args = type_template<T>::get_templates();

		if (builder_context::current_builder_context != nullptr) {
			if (builder_context::current_builder_context->gather_struct_decls) {
				auto map = builder_context::current_builder_context->gathered_struct_decls;
				// For now, assume types being extracted don't have template args	
				// TODO: use a combination of name and template args to deduplicate
				// extracted struct decls
				if (map->find(type->type_name) == map->end()) {
					auto sd = extract_struct_decl<T>();
					(*map)[type->type_name] =  sd;
				}
			}
		}

		return type;
	}
};

// Type specialization for basic C++ types
template <>
class type_extractor<short int> {
public:
	static block::type::Ptr extract_type(void) {
		block::scalar_type::Ptr type = std::make_shared<block::scalar_type>();
		type->scalar_type_id = block::scalar_type::SHORT_INT_TYPE;
		return type;
	}
};

template <>
class type_extractor<unsigned short int> {
public:
	static block::type::Ptr extract_type(void) {
		block::scalar_type::Ptr type = std::make_shared<block::scalar_type>();
		type->scalar_type_id = block::scalar_type::UNSIGNED_SHORT_INT_TYPE;
		return type;
	}
};

template <>
class type_extractor<int> {
public:
	static block::type::Ptr extract_type(void) {
		block::scalar_type::Ptr type = std::make_shared<block::scalar_type>();
		type->scalar_type_id = block::scalar_type::INT_TYPE;
		return type;
	}
};

template <>
class type_extractor<unsigned int> {
public:
	static block::type::Ptr extract_type(void) {
		block::scalar_type::Ptr type = std::make_shared<block::scalar_type>();
		type->scalar_type_id = block::scalar_type::UNSIGNED_INT_TYPE;
		return type;
	}
};
template <>
class type_extractor<long int> {
public:
	static block::type::Ptr extract_type(void) {
		block::scalar_type::Ptr type = std::make_shared<block::scalar_type>();
		type->scalar_type_id = block::scalar_type::LONG_INT_TYPE;
		return type;
	}
};

template <>
class type_extractor<unsigned long int> {
public:
	static block::type::Ptr extract_type(void) {
		block::scalar_type::Ptr type = std::make_shared<block::scalar_type>();
		type->scalar_type_id = block::scalar_type::UNSIGNED_LONG_INT_TYPE;
		return type;
	}
};
template <>
class type_extractor<long long int> {
public:
	static block::type::Ptr extract_type(void) {
		block::scalar_type::Ptr type = std::make_shared<block::scalar_type>();
		type->scalar_type_id = block::scalar_type::LONG_LONG_INT_TYPE;
		return type;
	}
};

template <>
class type_extractor<unsigned long long int> {
public:
	static block::type::Ptr extract_type(void) {
		block::scalar_type::Ptr type = std::make_shared<block::scalar_type>();
		type->scalar_type_id = block::scalar_type::UNSIGNED_LONG_LONG_INT_TYPE;
		return type;
	}
};

template <>
class type_extractor<char> {
public:
	static block::type::Ptr extract_type(void) {
		block::scalar_type::Ptr type = std::make_shared<block::scalar_type>();
		type->scalar_type_id = block::scalar_type::CHAR_TYPE;
		return type;
	}
};

template <>
class type_extractor<unsigned char> {
public:
	static block::type::Ptr extract_type(void) {
		block::scalar_type::Ptr type = std::make_shared<block::scalar_type>();
		type->scalar_type_id = block::scalar_type::UNSIGNED_CHAR_TYPE;
		return type;
	}
};

template <>
class type_extractor<float> {
public:
	static block::type::Ptr extract_type(void) {
		block::scalar_type::Ptr type = std::make_shared<block::scalar_type>();
		type->scalar_type_id = block::scalar_type::FLOAT_TYPE;
		return type;
	}
};

template <>
class type_extractor<double> {
public:
	static block::type::Ptr extract_type(void) {
		block::scalar_type::Ptr type = std::make_shared<block::scalar_type>();
		type->scalar_type_id = block::scalar_type::DOUBLE_TYPE;
		return type;
	}
};

template <>
class type_extractor<bool> {
public:
	static block::type::Ptr extract_type(void) {
		block::scalar_type::Ptr type = std::make_shared<block::scalar_type>();
		type->scalar_type_id = block::scalar_type::BOOL_TYPE;
		return type;
	}
};
// Type specialization for pointer type
template <typename T>
class type_extractor<T *> {
public:
	static block::type::Ptr extract_type(void) {
		block::pointer_type::Ptr type = std::make_shared<block::pointer_type>();
		type->pointee_type = type_extractor<T>::extract_type();
		return type;
	}
};
// Type specialization for reference type
template <typename T>
class type_extractor<T &> {
public:
	static block::type::Ptr extract_type(void) {
		block::reference_type::Ptr type = std::make_shared<block::reference_type>();
		type->referenced_type = type_extractor<T>::extract_type();
		return type;
	}
};

template <>
class type_extractor<void> {
public:
	static block::type::Ptr extract_type(void) {
		block::scalar_type::Ptr type = std::make_shared<block::scalar_type>();
		type->scalar_type_id = block::scalar_type::VOID_TYPE;
		return type;
	}
};

// Type specialization for array types
template <typename T, int x>
class type_extractor<T[x]> {
public:
	static block::type::Ptr extract_type(void) {
		block::array_type::Ptr type = std::make_shared<block::array_type>();
		type->element_type = type_extractor<T>::extract_type();
		type->size = x;
		return type;
	}
};

template <typename T>
class type_extractor<T[]> {
public:
	static block::type::Ptr extract_type(void) {
		block::array_type::Ptr type = block::to<block::array_type>(type_extractor<T[1]>::extract_type());
		type->size = -1;
		return type;
	}
};

template <typename T>
class type_extractor<const T> {
public:
	static block::type::Ptr extract_type(void) {
		block::type::Ptr type = type_extractor<T>::extract_type();
		type->is_const = true;
		return type;
	}

};

template <typename T>
class type_extractor<volatile T> {
public:
	static block::type::Ptr extract_type(void) {
		block::type::Ptr type = type_extractor<T>::extract_type();
		type->is_volatile = true;
		return type;
	}

};

template <typename T>
class type_extractor<const volatile T> {
public:
	static block::type::Ptr extract_type(void) {
		block::type::Ptr type = type_extractor<T>::extract_type();
		type->is_volatile = true;
		type->is_const = true;
		return type;
	}

};



// Type extractor for complete closure
template <typename T>
class type_extractor<dyn_var<T>> {
public:
	static block::type::Ptr extract_type(void) {
		block::builder_var_type::Ptr type = std::make_shared<block::builder_var_type>();
		type->builder_var_type_id = block::builder_var_type::DYN_VAR;
		type->closure_type = type_extractor<T>::extract_type();
		return type;
	}
};
template <typename T>
class type_extractor<static_var<T>> {
public:
	static block::type::Ptr extract_type(void) {
		block::builder_var_type::Ptr type = std::make_shared<block::builder_var_type>();
		type->builder_var_type_id = block::builder_var_type::STATIC_VAR;
		type->closure_type = type_extractor<T>::extract_type();
		return type;
	}
};


template <>
class type_extractor<generic> {
public:
	static block::type::Ptr extract_type(void) {
		// generic types don't actually have any types associated with them
		// and would be assigned a type separately
		return nullptr;	
	}
};



// Extracting function types
template <typename... args>
std::vector<block::type::Ptr> extract_type_vector_dyn(void);

template <typename T, typename... args>
std::vector<block::type::Ptr> extract_type_vector_helper_dyn(void) {
	std::vector<block::type::Ptr> rest = extract_type_vector_dyn<args...>();
	rest.push_back(type_extractor<T>::extract_type());
	return rest;
}

template <typename... args>
std::vector<block::type::Ptr> extract_type_vector_dyn(void) {
	return extract_type_vector_helper_dyn<args...>();
}

template <>
std::vector<block::type::Ptr> extract_type_vector_dyn<>(void);

template <typename r_type, typename... a_types>
class type_extractor<r_type(a_types...)> {
public:
	static block::type::Ptr extract_type(void) {
		block::function_type::Ptr type = std::make_shared<block::function_type>();
		type->return_type = type_extractor<r_type>::extract_type();
		type->arg_types = extract_type_vector_dyn<a_types...>();
		std::reverse(type->arg_types.begin(), type->arg_types.end());
		//type->arg_types = std::vector<block::type::Ptr>({type_extractor<a_types>::extract_type()...});
		return type;
	}
};

template <const char *N, typename... Args>
struct name {};

template <typename... Args>
struct extract_type_from_args;

template <typename T1, typename... Args>
struct extract_type_from_args<T1, Args...> {
	static std::vector<block::type::Ptr> get_types() {
		auto a = extract_type_from_args<Args...>::get_types();
		a.insert(a.begin(), type_extractor<T1>::extract_type());
		return a;
	}
};

template <>
struct extract_type_from_args<> {
	static std::vector<block::type::Ptr> get_types() {
		return std::vector<block::type::Ptr>();
	}
};

template <const char *N, typename... Args>
class type_extractor<name<N, Args...>> {
public:
	static block::type::Ptr extract_type(void) {
		block::named_type::Ptr type = std::make_shared<block::named_type>();
		type->type_name = N;
		type->template_args = extract_type_from_args<Args...>::get_types();
		return type;
	}
};


// Finally declare the create_block_type function

template <typename T>
block::type::Ptr dyn_var_impl<T>::create_block_type(void){
	return type_extractor<T>::extract_type();
}
template <typename...Args>
std::vector<block::type::Ptr> custom_type<Args...>::get_template_arg_types() {
	return extract_type_from_args<Args...>::get_types();
}


} // namespace builder

#endif
