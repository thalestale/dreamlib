#ifndef DREAMLIB_CHARACTER_H
#define DREAMLIB_CHARACTER_H

#include <godot_cpp/classes/audio_stream.hpp>
#include <godot_cpp/classes/audio_stream_player3d.hpp>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/shape_cast3d.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "gde_util.h"

namespace godot {

	class DreamCharacter : public CharacterBody3D {
		GDCLASS(DreamCharacter, CharacterBody3D)

	private:
		const StringName footstep_meta = "footstep";
		const StringName no_interaction = "nothing";

		const StringName input_key_up = "ui_up";
		const StringName input_key_down = "ui_down";
		const StringName input_key_right = "ui_right";
		const StringName input_key_left = "ui_left";

		StringName char_name = StringName("Unnamed");
		bool is_npc = true;

		Vector2 input_vec2;
		Node3D *c_camera;

		bool sharp_turn;
		double horizontal_speed;
		Vector3 linear_velocity;
		Vector3 horizontal_velocity;
		Vector3 horizontal_direction;
		Vector3 facing_mesh;
		Vector3 direction;
		Vector3 up_vec3;
		Vector3 gravity;
        Transform3D mesh_transform;

		Vector3 normal;
		Vector3 t;
		double x;
		double y;
		double ang;
		double s;
		double turn;
		double a;

		double sharp_turn_threshold;
		double deacceleration;
		double max_speed;
		double turn_speed;
		double acceleration;
		double walk_animation_blend;

		AudioStreamPlayer3D *footstep_player;
		Ref<AudioStream> default_footstep;
		double footsteps_volume;
		Node3D* armature;
		Node3D* floortype_check_raycaster;
		Object* floor_collider_node;
		
		ShapeCast3D *player_collision_ray;
		Object *node_collided;

	protected:
		static void _bind_methods();

	public:
		DreamCharacter();
		~DreamCharacter();

		void _notification(int p_what);
		
		//virtual void _default_interaction();

		double get_sharp_turn_threshold() const;
		double get_deacceleration() const;
		double get_max_speed() const;
		double get_turn_speed() const;
		double get_acceleration() const;
		double get_anim_blend() const;
		double get_horizontal_speed() const;
		Vector3 get_horizontal_velocity() const;
		Vector3 get_horizontal_direction() const;
		Vector3 get_direction() const;
		Vector3 get_linear_velocity() const;
		
		void set_direction(Vector3 value);
		void set_sharp_turn_threshold(double value);
		void set_deacceleration(double value);
		void set_max_speed(double value);
		void set_turn_speed(double value);
		void set_acceleration(double value);
		void set_horizontal_direction(Vector3 value);
		void set_horizontal_speed(double value);
		void set_horizontal_velocity(Vector3 value);
		void set_linear_velocity(Vector3 value);
		void set_gravity_value(Vector3 value);

		void play_footstep();
		AudioStreamPlayer3D* get_footstep_player() const;
		void set_footstep_player(AudioStreamPlayer3D* p_value);
		void set_default_footstep(Ref<AudioStream> p_value);
		Ref<AudioStream> get_default_footstep() const;
		Node3D* get_floortype_check_raycaster() const;
		void set_floortype_check_raycaster(Node3D* p_value);
		void set_footsteps_volume(double p_value);
		double get_footsteps_volume() const;

		Node3D* get_armature() const;
		void set_armature(Node3D* p_value);

		void compute_movement_vectors(double delta);

		bool is_sharp_turning() const;
		void move_character(double p_delta);
		Vector3 adjust_facing(Vector3 facing, Vector3 target, double step, double adjust_rate, Vector3 current_gn);

		ShapeCast3D* get_player_collision_ray();
		void set_player_collision_ray(ShapeCast3D* p_value);
		StringName get_interaction();

		void set_direction_from_key_input();
		void set_main_camera(Node3D *p_value);

		Vector3 get_random_direction() const;

		StringName get_char_name() const; 
		void set_char_name(StringName p_value);
		bool get_is_npc() const;
		void set_is_npc(bool p_value);

	};

}

#endif