#ifndef DREAMLIB_CAMERA_H
#define DREAMLIB_CAMERA_H

#include <godot_cpp/classes/camera3d.hpp>

namespace godot {
class DreamCamera : public Camera3D {
	GDCLASS(DreamCamera, Camera3D)

private:
	bool target_set;
	double translate_speed;
	Node3D *target;

	double translate_factor;
	Transform3D target_xform;
	Transform3D local_transform_only_origin;
	Transform3D local_transform_only_basis;
	Transform3D self_transform;

protected:
	static void _bind_methods();

public:
	DreamCamera();
	~DreamCamera();

	double get_translate_speed() const;
	void set_translate_speed(double value);
	void set_target(Node3D *target_node);
	Node3D *get_target() const;
	void set_player_as_target();

	bool is_target_set() const;
	void interpolate_position();
};
} //namespace godot

#endif
