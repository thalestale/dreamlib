#ifndef DREAMLIB_SCENE_CHANGER_H
#define DREAMLIB_SCENE_CHANGER_H

#include <godot_cpp/classes/animation.hpp>
#include <godot_cpp/classes/animation_player.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/shader_material.hpp>

namespace godot {

class DreamSceneChanger : public Node {
	GDCLASS(DreamSceneChanger, Node)

private:
	const StringName SCENE_LOADED = StringName("scene_loaded");
	const StringName SCENE_UNLOADED = StringName("scene_unloaded");
	const StringName SCENE_LIMBO = StringName("scene_limbo");
	const StringName TRANSITION_FINISHED = StringName("transition_finished");
	const StringName FADE_COMPLETE = StringName("fade_complete");
	const StringName TELEPORTED = StringName("teleported");
	const StringName FADE_ANIMATION = StringName("ShaderFade");
	const StringName NO_PATTERN = StringName("fade");
	const Callable call_fade_out_complete = Callable(this, StringName("fade_out_complete"));
	const Callable call_fade_in_complete = Callable(this, StringName("fade_in_complete"));

	const String PATTERN_TYPE = String("CompressedTexture2D");
	String PATTERN_PATH = "res://core/scene-manager/shader-patterns/%s.png";
	
	Dictionary default_options;
	ResourceLoader *pattern_loader;

	SceneTree *tree;

	ResourceLoader *scene_loader;
	bool is_scene_loading;
	String loading_scene_path;
	int scene_loader_thread_status;

	Ref<Animation> anim;
	AnimationPlayer *anim_player;
	ShaderMaterial *shader_mat;
	Node *shader_rect;
	bool fade_mode_in;
	bool fade_mode_out;

	double parsed_speed = 2.0;
	Color parsed_color = Color(0.0f, 0.0f, 0.0f);
	Ref<Resource> parsed_pattern_enter;
	Ref<Resource> parsed_pattern_leave;
	double parsed_wait_time = 0.1;
	bool parsed_invert_on_leave = true;
	double parsed_ease_enter = 1.0;
	double parsed_ease_leave = 1.0;
	bool parsed_no_scene_change = false;
	bool parsed_teleport = false;
	bool parsed_savepoint = false;

	enum OPTIONS_KEYS{
		SPEED,
		COLOR,
		PATTERN,
		WAIT_TIME,
		INVERT_ON_LEAVE,
		EASE,
		NO_SCENE_CHANGE,
		TELEPORT,
		SAVEPOINT,
		EASE_ENTER = 20,
		EASE_LEAVE = 21,
		PATTERN_ENTER = 22,
		PATTERN_LEAVE = 23
	};

protected:
	static void _bind_methods();

public:
	DreamSceneChanger();
	~DreamSceneChanger();

	void _notification(int p_what);
	void _process(double delta) override;

	void set_default_options(Dictionary p_value);
	Dictionary get_default_options() const;
	void set_pattern_texture_path(String p_value);
	String get_pattern_texture_path() const;

	void parse_options(Dictionary p_value);
	Ref<Resource> load_pattern(String p_value) const;

	void replace_scene(NodePath p_value);
	void replace_current_scene(NodePath p_value);

	void set_animation_player(AnimationPlayer *p_value);
	AnimationPlayer *get_animation_player() const;

	void set_shader_rect(Node *p_value);
	Node *get_shader_rect() const;

	void fade_out();
	void fade_in();
	void fade_out_complete(StringName p_value);
	void fade_in_complete(StringName p_value);

	double get_parsed_wait_time() const;
	bool get_parsed_teleport() const;
	bool get_parsed_no_scene_change() const;

	void same_scene_fade_out();
	void same_scene_fade_in();
};
} //namespace godot
#endif
