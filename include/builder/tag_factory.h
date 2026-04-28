#ifndef BUILDER_TAG_FACTORY_H
#define BUILDER_TAG_FACTORY_H

#include "util/tracer.h"

namespace builder {

static bool is_factory_frozen(void);
class true_top;
class builder_union_top;

class tag_factory {
	std::unordered_map<tracer::tag, tracer::tag_id> internal_map;
	tracer::tag_id next_id = 1;
public:
	tracer::tag_id create_tag_id_helper (const tracer::tag& t) {
		if (is_factory_frozen()) return 0;
		auto it = internal_map.find(t);
		if (it != internal_map.end()) 
			return it->second;
		internal_map[t] = next_id++;
		return next_id - 1;
	}
	tracer::tag_id create_tag_id (const tracer::tag& t) { return create_tag_id_helper(t); }

	friend void purge_markers_from_nd_state(const std::vector<builder_union_top*>& marker_values);
};

}


#endif
