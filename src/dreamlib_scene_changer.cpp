#include "dreamlib_scene_changer.h"
#include "gde_util.h"
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/core/class_db.hpp>


using namespace godot;

void DreamSceneChanger::_bind_methods() {
    ClassDB::bind_method(D_METHOD("parse_options", "p_value"), &DreamSceneChanger::parse_options);
    ClassDB::bind_method(D_METHOD("load_pattern", "p_value"), &DreamSceneChanger::load_pattern);
    ClassDB::bind_method(D_METHOD("replace_scene", "p_value"), &DreamSceneChanger::replace_scene);
    ClassDB::bind_method(D_METHOD("fade_out"), &DreamSceneChanger::fade_out);
    ClassDB::bind_method(D_METHOD("fade_in"), &DreamSceneChanger::fade_in);
    ClassDB::bind_method(D_METHOD("fade_out_complete", "p_value"), &DreamSceneChanger::fade_out_complete);
    ClassDB::bind_method(D_METHOD("fade_in_complete", "p_value"), &DreamSceneChanger::fade_in_complete);
    ClassDB::bind_method(D_METHOD("get_parsed_wait_time"), &DreamSceneChanger::get_parsed_wait_time);
    ClassDB::bind_method(D_METHOD("get_parsed_teleport"), &DreamSceneChanger::get_parsed_teleport);
    ClassDB::bind_method(D_METHOD("get_parsed_no_scene_change"), &DreamSceneChanger::get_parsed_no_scene_change);
    ClassDB::bind_method(D_METHOD("same_scene_fade_out"), &DreamSceneChanger::same_scene_fade_out);
    ClassDB::bind_method(D_METHOD("same_scene_fade_in"), &DreamSceneChanger::same_scene_fade_in);
    EXPORT_PROPERTY(STRING, pattern_texture_path, DreamSceneChanger);

    ClassDB::bind_method(D_METHOD("set_" "default_options", "p_value"), &DreamSceneChanger::set_default_options);
    ClassDB::bind_method(D_METHOD("get_" "default_options"), &DreamSceneChanger::get_default_options);
    ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "default_options", PROPERTY_HINT_DICTIONARY_TYPE), "set_" "default_options", "get_" "default_options");

    ClassDB::bind_method(D_METHOD("set_" "shader_rect", "p_value"), &DreamSceneChanger::set_shader_rect); 
    ClassDB::bind_method(D_METHOD("get_" "shader_rect"), &DreamSceneChanger::get_shader_rect); 
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "shader_rect", PROPERTY_HINT_NODE_TYPE, "Node"), "set_" "shader_rect", "get_" "shader_rect");

    ClassDB::bind_method(D_METHOD("set_" "animation_player", "p_value"), &DreamSceneChanger::set_animation_player);
    ClassDB::bind_method(D_METHOD("get_" "animation_player"), &DreamSceneChanger::get_animation_player); 
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "animation_player", PROPERTY_HINT_NODE_TYPE, "Node"), "set_" "animation_player", "get_" "animation_player");

    ADD_SIGNAL(MethodInfo("scene_loaded"));
    ADD_SIGNAL(MethodInfo("scene_unloaded"));
    ADD_SIGNAL(MethodInfo("fade_complete"));
    ADD_SIGNAL(MethodInfo("transition_finished"));
    ADD_SIGNAL(MethodInfo("scene_limbo"));
    ADD_SIGNAL(MethodInfo("teleported"));
}

DreamSceneChanger::DreamSceneChanger() {
    default_options.set(SPEED, 2.0);
    default_options.set(COLOR, Color(0.0f, 0.0f, 0.0f));
	default_options.set(PATTERN, NO_PATTERN);
	default_options.set(WAIT_TIME, 0.1);
	default_options.set(INVERT_ON_LEAVE, true);
	default_options.set(EASE, 1.0);
	default_options.set(NO_SCENE_CHANGE, false);
	default_options.set(TELEPORT, false);
	default_options.set(SAVEPOINT, false);

    pattern_loader = memnew(ResourceLoader);

    tree = nullptr;
 
    scene_loader = memnew(ResourceLoader);
    is_scene_loading = false;
    loading_scene_path = String("");

    anim_player = nullptr;
    shader_mat = nullptr;
    shader_rect = nullptr;
    fade_mode_in = true;
    fade_mode_out = true;
}

DreamSceneChanger::~DreamSceneChanger() {}

void godot::DreamSceneChanger::_notification(int p_what){
    if (p_what == NOTIFICATION_READY){
        tree = get_tree();
        anim = anim_player->get_animation(FADE_ANIMATION);
        shader_mat = cast_to<ShaderMaterial>(shader_rect->call("get_material"));
        default_options.make_read_only();
        call_deferred("emit_signal", TELEPORTED);
        call_deferred("emit_signal", SCENE_LOADED);
        call_deferred("emit_signal", TRANSITION_FINISHED);
    } 
}

void DreamSceneChanger::_process(double delta){
    if (is_scene_loading){
        scene_loader_thread_status = scene_loader->load_threaded_get_status(loading_scene_path);
        if (scene_loader_thread_status == 3){
            is_scene_loading = false;
            replace_current_scene(loading_scene_path);
        }
    }
}

void DreamSceneChanger::set_default_options(Dictionary p_value){
    default_options = p_value;
}

Dictionary DreamSceneChanger::get_default_options() const{
    return default_options;
}

void DreamSceneChanger::set_pattern_texture_path(String p_value) {
    PATTERN_PATH = p_value;
}

String DreamSceneChanger::get_pattern_texture_path() const {
    return PATTERN_PATH;
}

void DreamSceneChanger::parse_options(Dictionary p_value){
    // for(int i = 0; i < default_options.size(); i++){
    //     if (!p_value.has(default_options_keys[i])){
    //         p_value[default_options_keys[i]] = default_options[default_options_keys[i]];
    //     }
    // }
    p_value.merge(default_options);
    p_value.get_or_add(PATTERN_LEAVE, p_value[PATTERN]);
    p_value[PATTERN_LEAVE] = load_pattern(p_value[PATTERN_LEAVE]);
    p_value.get_or_add(PATTERN_ENTER, p_value[PATTERN]);
    p_value[PATTERN_ENTER] = load_pattern(p_value[PATTERN_ENTER]);

    parsed_ease_enter = p_value.get_or_add(EASE_ENTER, p_value[EASE]);
    parsed_ease_leave = p_value.get_or_add(EASE_LEAVE, p_value[EASE]);
    parsed_speed = p_value[SPEED];
    parsed_color = p_value[COLOR];
    parsed_wait_time = p_value[WAIT_TIME];
    parsed_invert_on_leave = p_value[INVERT_ON_LEAVE];
    parsed_no_scene_change = p_value[NO_SCENE_CHANGE];
    parsed_teleport = p_value[TELEPORT];
    parsed_savepoint = p_value[SAVEPOINT];

    if (p_value[PATTERN_LEAVE] == NO_PATTERN){
        fade_mode_in = true;
    } else {
        fade_mode_in = false;
        parsed_pattern_leave = p_value[PATTERN_LEAVE];
    }
    
    if (p_value[PATTERN_ENTER] == NO_PATTERN){
        fade_mode_out = true;
    } else {
        fade_mode_out = false;
        parsed_pattern_enter = p_value[PATTERN_ENTER];
    }
}

Ref<Resource> DreamSceneChanger::load_pattern(String p_value) const{
    if (p_value.is_absolute_path()){
        return pattern_loader->load(p_value, PATTERN_TYPE);
    }
    else if(p_value == NO_PATTERN){
        return nullptr;
    }
    return pattern_loader->load(PATTERN_PATH % p_value, PATTERN_TYPE);
}

void DreamSceneChanger::replace_scene(NodePath p_value) {
    loading_scene_path = p_value;
    scene_loader->load_threaded_request(p_value, "PackedScene");
    is_scene_loading = true;
    emit_signal(SCENE_UNLOADED);
}

void DreamSceneChanger::replace_current_scene(NodePath p_value){
    tree->change_scene_to_packed(scene_loader->load_threaded_get(p_value));
    emit_signal(SCENE_LIMBO);
}

void DreamSceneChanger::set_animation_player(AnimationPlayer *p_value){
    anim_player = p_value;
}

AnimationPlayer *DreamSceneChanger::get_animation_player() const{
    return anim_player;
}

void DreamSceneChanger::set_shader_rect(Node *p_value){
    shader_rect = p_value;
}

Node *DreamSceneChanger::get_shader_rect() const{
    return shader_rect;
}

void DreamSceneChanger::fade_out(){
    anim_player->set_speed_scale(parsed_speed);
    if (!fade_mode_out){
        shader_mat->set_shader_parameter(StringName("dissolve_texture"), parsed_pattern_enter);
    }
    shader_mat->set_shader_parameter(StringName("fade"), fade_mode_out);
    shader_mat->set_shader_parameter(StringName("fade_color"), parsed_color);
    shader_mat->set_shader_parameter(StringName("inverted"), false);
    anim->track_set_key_transition(0.0, 0.0, parsed_ease_enter);
    anim_player->connect(StringName("animation_finished"), call_fade_out_complete, ConnectFlags::CONNECT_ONE_SHOT);
    anim_player->play(FADE_ANIMATION);
    
}
void DreamSceneChanger::fade_out_complete(StringName p_value){
    emit_signal(FADE_COMPLETE);
}

void DreamSceneChanger::fade_in_complete(StringName p_value){
    emit_signal(TRANSITION_FINISHED);
}

double DreamSceneChanger::get_parsed_wait_time() const {
	return parsed_wait_time;
}

bool DreamSceneChanger::get_parsed_teleport() const {
	return parsed_teleport;
}

bool DreamSceneChanger::get_parsed_no_scene_change() const {
	return parsed_no_scene_change;
}


void DreamSceneChanger::fade_in(){
    if (!fade_mode_in){
        shader_mat->set_shader_parameter(StringName("dissolve_texture"), parsed_pattern_leave);
    }
    shader_mat->set_shader_parameter(StringName("fade"), fade_mode_in);
    shader_mat->set_shader_parameter(StringName("inverted"), parsed_invert_on_leave);
    anim->track_set_key_transition(0.0, 0.0, parsed_ease_leave);
    anim_player->connect(StringName("animation_finished"), call_fade_in_complete, ConnectFlags::CONNECT_ONE_SHOT);
    anim_player->play_backwards(FADE_ANIMATION);
}

void DreamSceneChanger::same_scene_fade_out() {
    anim_player->set_speed_scale(3.0);
    shader_mat->set_shader_parameter(StringName("dissolve_texture"), load_pattern("horizontal"));
    shader_mat->set_shader_parameter(StringName("fade"), false);
    shader_mat->set_shader_parameter(StringName("fade_color"), Color(0.0f, 0.0f, 0.0f));
    shader_mat->set_shader_parameter(StringName("inverted"), false);
    anim->track_set_key_transition(0.0, 0.0, 2.0);
    anim_player->connect(StringName("animation_finished"), call_fade_out_complete, ConnectFlags::CONNECT_ONE_SHOT);
    anim_player->play(FADE_ANIMATION);
}

void DreamSceneChanger::same_scene_fade_in() {
    shader_mat->set_shader_parameter(StringName("inverted"), true);
    anim_player->connect(StringName("animation_finished"), call_fade_in_complete, ConnectFlags::CONNECT_ONE_SHOT);
    anim_player->play_backwards(FADE_ANIMATION);
}
