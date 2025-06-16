#ifndef GDE_UTIL_H
#define GDE_UTIL_H

#define BIND_GETTER(METHOD_NAME, CLASS_NAME) ClassDB::bind_method(D_METHOD("get_" #METHOD_NAME), &CLASS_NAME::get_##METHOD_NAME)
#define BIND_SETTER(METHOD_NAME, CLASS_NAME, ARGS) ClassDB::bind_method(D_METHOD("set_" #METHOD_NAME, ARGS), &CLASS_NAME::set_##METHOD_NAME)

#define EXPORT_PROPERTY(type, name, scriptclass) \
	BIND_SETTER(name, scriptclass, "p_value"); \
	BIND_GETTER(name, scriptclass); \
	ADD_PROPERTY(PropertyInfo(Variant::type, #name), "set_" #name, "get_" #name)

#define DEF_GETSET(type, name) \
	type get_##name() const; \
	void set_##name(##type p_value);

#define DEC_GETSET(type, name, scriptclass) \
	type scriptclass::get_##name() const { \
	  return name; \
	} \
	void scriptclass::set_##name(type p_value) { \
	  name = p_value; \
	}

#endif