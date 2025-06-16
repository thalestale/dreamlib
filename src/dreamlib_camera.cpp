#include "dreamlib_camera.h"

#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void DreamCamera::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_translate_speed"), &DreamCamera::get_translate_speed);
	ClassDB::bind_method(D_METHOD("set_translate_speed", "value"), &DreamCamera::set_translate_speed);
	ClassDB::bind_method(D_METHOD("is_target_set"), &DreamCamera::is_target_set);
	ClassDB::bind_method(D_METHOD("set_target", "target_node"), &DreamCamera::set_target);
	ClassDB::bind_method(D_METHOD("get_target"), &DreamCamera::get_target);
	ClassDB::bind_method(D_METHOD("set_player_as_target"), &DreamCamera::set_player_as_target);
	ClassDB::bind_method(D_METHOD("interpolate_position"), &DreamCamera::interpolate_position);

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "translate_speed"), "set_translate_speed", "get_translate_speed");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "target", PROPERTY_HINT_NODE_TYPE, "Node3D"), "set_target", "get_target");
}

DreamCamera::DreamCamera() {
	target_set = false;
	translate_speed = 95.0;
	target = nullptr;
}

DreamCamera::~DreamCamera() {}

void DreamCamera::interpolate_position() {
	if (target_set == true) {
		self_transform = this->get_global_transform();

		translate_factor = (1.0 - Math::pow(1.0 - Math::smoothstep(0.0, 100.0, translate_speed * 1.01), get_physics_process_delta_time()));

		target_xform = target->get_global_transform();
		local_transform_only_origin = Transform3D(Basis(), self_transform.origin);
		local_transform_only_basis = Transform3D(self_transform.basis, Vector3());

		local_transform_only_origin = local_transform_only_origin.interpolate_with(target_xform, translate_factor);

		this->set_global_transform(Transform3D(local_transform_only_basis.basis, local_transform_only_origin.origin).orthonormalized());
	}

}

double DreamCamera::get_translate_speed() const {
	return translate_speed;
}

void DreamCamera::set_translate_speed(double value) {
	translate_speed = value;
}

bool DreamCamera::is_target_set() const {
	return target_set;
}

void DreamCamera::set_target(Node3D* target_node) {
	target = target_node;
	if (target_node != nullptr){
		target_set = true;
	}
}

Node3D* DreamCamera::get_target() const {
	return target;
}

void DreamCamera::set_player_as_target() {
	SceneTree* tree = this->godot::Node::get_tree();
	Node* player_node = tree->get_first_node_in_group("PlayerNode");
	if (player_node->find_child("target", false) == nullptr) {
		target = memnew(Node3D);
		player_node->add_child(target);
		target->set_global_transform(this->get_global_transform());
	}
	else {
		target = player_node->get_node<Node3D>("target");
		target->set_global_transform(this->get_global_transform());
	}
	target_set = true;
}