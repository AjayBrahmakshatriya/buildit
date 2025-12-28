#ifndef BUILDER_UNION_H
#define BUILDER_UNION_H

#include "builder/dyn_var.h"
#include "builder/static_var.h"
#include "builder/nd_var.h"

namespace builder {

template <typename T>
class builder_union;

template <typename T, typename TR=typename std::remove_reference<T>::type>
struct dyn_var_to_union {};

template <typename T, typename TR>
struct dyn_var_to_union<T, dyn_var<TR>> {
	using type = builder_union<TR>;
};


// builder_union doesn't inherit
// from dyn_var but wraps around it
template <typename T>
class builder_union {
public:
	dyn_var<T> var;
	static_var<T> cval;
	enum mode {
		STATIC,
		DYNAMIC
	} var_mode;
	
	nd_var<true_top> needs_dynamic = true_top::F;

	// Maximum states this variable can take 
	// before it is promoted to dynamic
	constexpr static int assign_count_max = 16;
	int assign_count = 0;

	// This is the behavior for most
	// constructor except when explicitly initialized
	// with a dynamic value
	// Args are only used for dynamic mode, if the mode is static, 
	// the user has to initialize after
	template <typename...Args>
	void check_and_init(const Args&...args) {
		// First try to guess if this needs dynamic in the future
		// var_mode never changes so we never need to reinit different state
		// It probably makes sense to put the variables into a variant 
		// TODO: Future improvement
		if (needs_dynamic.get()->value == true_top::T) {
			var_mode = DYNAMIC;
			// Regular defer init the dynamic variable
			var.deferred_init(args...);
		} else {
			var_mode = STATIC;
			// Regular defer init the static_variable
			cval.deferred_init();
			//cval needs to be initialized outside
		}
		
	}

public:


	
	builder_union(): var(defer_init()), cval(defer_init()) {
		check_and_init();
		// Uninitialized
	}

	builder_union(const builder_union& other): var(defer_init()), cval(defer_init()) {
		// If RHS is dynamic, we are dynamic too, no need to look at guesses
		if (other.var_mode == DYNAMIC) {
			var_mode = DYNAMIC;
			var.deferred_init(other.var);
		} else {
			// If RHS is static, we do the regular init
			check_and_init(other.cval);
			if (var_mode == STATIC) {
				cval = other.cval;
			}
		}
	}			



	// If it is a dyn_var, there is no check
	template <typename TO>
	builder_union(const dyn_var<TO>& other_var): var(other_var), cval(defer_init()) {
		var_mode = DYNAMIC;
	}
	// With block var works exactly the same, but it also sets the type
	builder_union(const with_block_var& bv): var(bv), cval(defer_init()) {
		var_mode = DYNAMIC;
		bv.var->var_type = create_block_type();
		bv.var->var_type->static_offset = bv.var->static_offset;
	}

	template <typename TO>
	builder_union(dyn_var<TO>&& other_var): var(std::move(other_var)), cval(defer_init()) {
		var_mode = DYNAMIC;
	}
	
	// If it is a constant, 
	builder_union(const T& v): var(defer_init()), cval(defer_init()) {
		check_and_init(v);
		if (var_mode == STATIC) {
			cval = v;
		}	
	}
	
	builder_union& operator=(const builder_union& other) {
		assign_count++;

		if (var_mode == STATIC) {
			if (assign_count > assign_count_max) {
				needs_dynamic.require_val(true_top::T);
				// This assert should never hit
				assert(false && "Static variable hit the max limit");		
			}			
			if (other.var_mode == DYNAMIC) {
				// This is a contradiction, we can simply 
				// propogate the information above
				needs_dynamic.require_val(true_top::T);
				// This assert should never hit
				assert(false && "Static variable cannot be assigned from dynamic");	
			}	
			// LHS and RHS are both static, 
			cval = other.cval;
		} else {
			// If we are dynamic, there is not checks
			if (other.var_mode == STATIC) {
				var = other.cval;
			} else {
				var = other.var;
			}
		}

		return *this;
	}
	explicit operator bool () {
		if (var_mode == STATIC) return (bool) cval;
		else return (bool) var;
	}
public:
	// Same API as dyn_var so the signature extract works
	static block::type::Ptr create_block_type(void) {
		return type_extractor<T>::extract_type();
	}
public:
	
	template <typename T1>
	friend block::expr::Ptr to_expr(const builder_union<T1>& d);
};

// to_expr overload
template <typename T>
block::expr::Ptr to_expr(const builder_union<T>& d) {
	if (d.var_mode == builder_union<T>::STATIC) {
		return to_expr(d.cval);
	} else {
		return to_expr(d.var);
	}
}

template <typename T1, typename T2>
struct allowed_union_types{};

template <typename T1, typename T2>
struct allowed_union_types<builder_union<T1>, builder_union<T2>> {
	using type = void;
	using up_type1 = const builder_union<T1>&;
	using up_type2 = const builder_union<T2>&;
};
template <typename T1, typename T2>
struct allowed_union_types<builder_union<T1>, T2> {
	using type = void;
	using up_type1 = const builder_union<T1>&;
	using up_type2 = builder_union<T2>;
};
template <typename T1, typename T2>
struct allowed_union_types<T1, builder_union<T2>> {
	using type = void;
	using up_type1 = builder_union<T1>;
	using up_type2 = const builder_union<T2>&;
};


#define BINARY_OPERATOR(op, op_name) \
template <typename T1, typename T2,  \
	typename RetType =  \
	typename dyn_var_to_union<decltype(std::declval<dyn_var<T1>>() op std::declval<dyn_var<T2>>())>::type> \
auto bunion_##op_name (const builder_union<T1>& d1, const builder_union<T2>& d2) -> RetType& { \
	/* The special case is if both sides are static*/ \
	if (d1.var_mode == builder_union<T1>::STATIC && d2.var_mode == builder_union<T2>::STATIC) { \
		return *get_invocation_state()->get_arena()->allocate<RetType>(std::move(d1.cval op d2.cval)); \
	} else if (d1.var_mode == builder_union<T1>::STATIC && d2.var_mode == builder_union<T2>::DYNAMIC) { \
		return *get_invocation_state()->get_arena()->allocate<RetType>(std::move(d1.cval op d2.var)); \
	} else if (d1.var_mode == builder_union<T1>::DYNAMIC && d2.var_mode == builder_union<T2>::STATIC) { \
		return *get_invocation_state()->get_arena()->allocate<RetType>(std::move(d1.var op d2.cval)); \
	} else { \
		return *get_invocation_state()->get_arena()->allocate<RetType>(std::move(d1.var op d2.var)); \
	} \
} \
template <typename T1, typename T2, typename V=allowed_union_types<T1, T2>, typename V2=typename V::type, \
	typename RetType = decltype(bunion_##op_name(typename V::up_type1{}, typename V::up_type2{}))> \
auto operator op (const T1& d1, const T2& d2) -> RetType { \
	return bunion_##op_name((typename V::up_type1)d1, (typename V::up_type2)d2); \
} 

#define UNARY_OPERATOR(op, op_name) \
template <typename T1, typename RetType = typename dyn_var_to_union<decltype(op std::declval<dyn_var<T1>>())>::type> \
auto operator op (const builder_union<T1>& d1) -> RetType& { \
	if (d1.var_mode == builder_union<T1>::STATIC) { \
		return *get_invocation_state()->get_arena()->allocate<RetType>(std::move(op d1.cval)); \
	} else { \
		return *get_invocation_state()->get_arena()->allocate<RetType>(std::move(op d1.var)); \
	} \
}

// Binary operators
// Arithmetic Operators
BINARY_OPERATOR(+, plus_expr)
BINARY_OPERATOR(-, minus_expr)
BINARY_OPERATOR(*, mul_expr)
BINARY_OPERATOR(/, div_expr)
BINARY_OPERATOR(%, mod_expr)
// Relational Operators
BINARY_OPERATOR(==, equals_expr)
BINARY_OPERATOR(!=, ne_expr)
BINARY_OPERATOR(<, lt_expr)
BINARY_OPERATOR(>, gt_expr)
BINARY_OPERATOR(<=, lte_expr)
BINARY_OPERATOR(>=, gte_expr)
// Bitwise Operators
BINARY_OPERATOR(&, bitwise_and_expr)
BINARY_OPERATOR(|, bitwise_or_expr)
BINARY_OPERATOR(^, bitwise_xor_expr)
BINARY_OPERATOR(<<, lshift_expr)
BINARY_OPERATOR(>>, rshift_expr)
// Logical Operators
BINARY_OPERATOR(&&, and_expr)
BINARY_OPERATOR(||, or_expr)

// Unary Operators
// Arithmetic Operators
UNARY_OPERATOR(-, unary_minus_expr)
// Bitwise Operators
UNARY_OPERATOR(~, bitwise_not_expr)
// Logical Operators
UNARY_OPERATOR(!, not_expr)
// Other Operators
UNARY_OPERATOR(&, addr_of_expr)

#undef BINARY_OPERATOR
#undef UNARY_OPERATOR

} // namespace builder

#endif
