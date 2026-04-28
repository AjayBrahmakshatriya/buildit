#ifndef ND_VAR_H
#define ND_VAR_H

#include "builder/static_var.h"
#include "util/tracer.h"
#include "builder/exceptions.h"
namespace builder {

/* The new API for nd_var-able objects is simple
   We require the objects to follow data-flow lattice properties
   Each ND var wrapped T can have whatever state it wants, but it needs to derive from 
   nd_node_base (we don't need to have separate generators any more).
   nd_node_base objects will go across executions and will be stored
   in the invocation state. 

   The type T however needs to provide two functions : check(int) which checks if 
   a new value satisfies the current state of the object. merge(int) merges the new 
   value into the current state and also updates the generator.
   
*/

// Base class for nd_var wrappable objects
// We are not making any functions virtual but we declare them here to make sure 
// users declare them too
class nd_node_base {
protected: 
	nd_node_base() = default;
public:
	using value_type = void;

	bool check(int e) {
		assert(false && "Every derived type must define check");
		return false;
	}
	bool merge(int e) {
		assert(false && "Every deried type must define merge");
	}	
	virtual ~nd_node_base() {}
};

template <typename Derived>
class nd_node: public nd_node_base {
protected:
	nd_node() = default;
public:
	using derived_type = Derived;
	std::vector<std::weak_ptr<Derived>> dependents;

	void add_dependent(const std::shared_ptr<Derived>& dep) {
		dependents.push_back(dep);
	}

	template <typename ValueType>
	void propagate_requires_atleast(ValueType e) {
		Derived* self = static_cast<Derived*>(this);
		if (self->check(e)) return;
		self->merge(e);
		self->changed();
		auto new_value = self->get_value();
		for (auto& weak_dep : dependents) {
			auto dep = weak_dep.lock();
			if (!dep) continue;
			dep->propagate_requires_atleast(new_value);
		}
	}

	void changed() {}
	static void finalize_updates() {}
};

template <typename T, typename...Args>
std::shared_ptr<T> get_or_create_generator(tracer::tag req_tag, Args&&...args) {
	if (get_invocation_state()->nd_state_map.find(req_tag) == get_invocation_state()->nd_state_map.end()) {
		get_invocation_state()->nd_state_map[req_tag] = std::make_shared<T>(std::forward<Args>(args)...);
	}
	return std::static_pointer_cast<T>(get_invocation_state()->nd_state_map[req_tag]);
}

// A simple true at top boolean nd_var wrappable type
class true_top: public nd_node<true_top> {
public:
	typedef enum {
		T = 1,
		F = 0,
	} value_t;

	value_t value;
	
	using value_type = value_t;
	
	true_top(value_t def): value(def) {}
	true_top(): value(F) {}
	value_t get_value() const { return value; }

	bool check(value_t e) {
		if (value == T) return true;
		if (e == value) return true;
		return false;
	}
	void merge(value_t e) {
		if (e == F) return;
		value = e;
	}
};

template <typename T>
class nd_var {
	static_assert(std::is_base_of<nd_node_base, T>::value, "Types wrapped in nd_var must derive from nd_node_base");
	std::shared_ptr<T> val;	
	tracer::tag t_cached;
private:
public:
	nd_var() {
		tracer::tag t = tracer::get_offset_for_nd_var();
		t_cached = t;
		val = get_or_create_generator<T>(t);
	}
	template <typename...Args>
	nd_var(Args&&...args) {
		tracer::tag t = tracer::get_offset_for_nd_var();
		t_cached = t;
		val = get_or_create_generator<T>(t, std::forward<Args>(args)...);
	}

	// Allow access to wrapped value in case user wants to access object specific APIs	
	operator T& (void) {
		return *val;
	}
	operator const T& (void) const {
		return *val;
	}

	T* get(void) {
		return val.get();
	}
	const T* get(void) const {
		return val.get();
	}
	void require_val(typename T::value_type e) {
		// If the required value is compatible with the current state, 
		// return 
		if (val->check(e)) return;
		// Otherwise, merge update and throw
		val->propagate_requires_atleast(e);
		T::finalize_updates();
		throw NonDeterministicFailureException();
	}
	void require_val(const nd_var<T>& other) {
		other.val->add_dependent(val);
		if (val->check(other.val->get_value())) return;
		val->propagate_requires_atleast(other.val->get_value());
		T::finalize_updates();
		throw NonDeterministicFailureException();
	}

	void require_val_no_throw (typename T::value_type e) {
		// If the required value is compatible with the current state, 
		// return 
		if (val->check(e)) return;
		// Otherwise, record that this run also didn't succeed
		// and we need a rerun
		get_run_state()->set_needs_nd_rerun();
	}
};

}



#endif
