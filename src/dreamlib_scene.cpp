#if 0

#include "dreamlib_scene.h"
#include "gde_util.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

using namespace godot;


void DreamScene::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_scene_name"), &DreamScene::get_scene_name);
	ClassDB::bind_method(D_METHOD("get_default_bgm_volume"), &DreamScene::get_default_bgm_volume);
	ClassDB::bind_method(D_METHOD("get_default_bgm_file_name"), &DreamScene::get_default_bgm_file_name);
	ClassDB::bind_method(D_METHOD("get_default_bgm_fade"), &DreamScene::get_default_bgm_fade);
	ClassDB::bind_method(D_METHOD("get_default_ambience_volume"), &DreamScene::get_default_ambience_volume);
	ClassDB::bind_method(D_METHOD("get_default_ambience_file_name"), &DreamScene::get_default_ambience_file_name);
	ClassDB::bind_method(D_METHOD("get_default_ambience_fade"), &DreamScene::get_default_ambience_fade);
	ClassDB::bind_method(D_METHOD("get_player"), &DreamScene::get_player);
	ClassDB::bind_method(D_METHOD("is_inventory_key_permited"), &DreamScene::is_inventory_key_permited);

	ClassDB::bind_method(D_METHOD("set_scene_name", "p_name_value"), &DreamScene::set_scene_name);
	ClassDB::bind_method(D_METHOD("set_default_bgm_volume", "p_volume"), &DreamScene::set_default_bgm_volume);
	ClassDB::bind_method(D_METHOD("set_default_bgm_file_name", "p_name_value"), &DreamScene::set_default_bgm_file_name);
	ClassDB::bind_method(D_METHOD("set_default_bgm_fade", "p_fade"), &DreamScene::set_default_bgm_fade);
	ClassDB::bind_method(D_METHOD("set_default_ambience_volume", "p_volume"), &DreamScene::set_default_ambience_volume);
	ClassDB::bind_method(D_METHOD("set_default_ambience_file_name", "p_name_value"), &DreamScene::set_default_ambience_file_name);
	ClassDB::bind_method(D_METHOD("set_default_ambience_fade", "p_fade"), &DreamScene::set_default_ambience_fade);
	ClassDB::bind_method(D_METHOD("set_player", "p_player"), &DreamScene::set_player);
	ClassDB::bind_method(D_METHOD("set_inventory_key_permited", "p_enable_value"), &DreamScene::set_inventory_key_permited);

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "scene_name"), "set_scene_name", "get_scene_name");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "inventory_key_permited"), "set_inventory_key_permited", "is_inventory_key_permited");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "player_node", PROPERTY_HINT_NODE_TYPE, "DreamCharacter"), "set_player", "get_player");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "default_bgm_file_name"), "set_default_bgm_file_name", "get_default_bgm_file_name");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "default_bgm_volume"), "set_default_bgm_volume", "get_default_bgm_volume");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "default_bgm_fade"), "set_default_bgm_fade", "get_default_bgm_fade");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "default_ambience_file_name"), "set_default_ambience_file_name", "get_default_ambience_file_name");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "default_ambience_volume"), "set_default_ambience_volume", "get_default_ambience_volume");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "default_ambience_fade"), "set_default_ambience_fade", "get_default_ambience_fade");
}

DreamScene::DreamScene() {
	scene_name = String("New World");
	default_bgm_file_name = "";
	default_bgm_volume = -0.3;
	default_bgm_fade = 1.0;
	default_ambience_file_name = "";
	default_ambience_volume = -12.0;
	default_ambience_fade = 3.0;

	inventory_key_permited = true;
	player_node = nullptr;

}

DreamScene::~DreamScene() {}

// void DreamScene::_notification(int p_what) {
// 	switch (p_what){
// 	case NOTIFICATION_READY:
// 		break;
	
// 	default:
// 		break;
// 	}
// }

String DreamScene::get_scene_name() const {
	return scene_name;
}

bool DreamScene::is_inventory_key_permited() const {
	return inventory_key_permited;
}

void DreamScene::set_scene_name(String p_name_value) {
	scene_name = p_name_value;
}

void DreamScene::set_inventory_key_permited(bool p_enable_value) {
	inventory_key_permited = p_enable_value;
}

void DreamScene::set_default_bgm_file_name(String p_name_value) {
	default_bgm_file_name = p_name_value;
}

void DreamScene::set_default_bgm_volume(double p_volume) {
	default_bgm_volume = p_volume;
}

void DreamScene::set_default_bgm_fade(double p_fade) {
	default_bgm_fade = p_fade;
}

double DreamScene::get_default_bgm_volume() const {
	return default_bgm_volume;
}

double DreamScene::get_default_bgm_fade() const {
	return default_bgm_fade;
}

String DreamScene::get_default_bgm_file_name() const {
	return default_bgm_file_name;
}

void DreamScene::set_default_ambience_file_name(String p_name_value) {
	default_ambience_file_name = p_name_value;
}

void DreamScene::set_default_ambience_volume(double p_volume) {
	default_ambience_volume = p_volume;
}

void DreamScene::set_default_ambience_fade(double p_fade) {
	default_ambience_fade = p_fade;
}

double DreamScene::get_default_ambience_volume() const {
	return default_ambience_volume;
}

double DreamScene::get_default_ambience_fade() const {
	return default_ambience_fade;
}

String DreamScene::get_default_ambience_file_name() const {
	return default_ambience_file_name;
}

DreamCharacter* DreamScene::get_player() {
	if (player_node != nullptr) {
		return player_node;
	}
	else if (!Engine::get_singleton()->is_editor_hint()) {
		if (get_tree()->has_group("PlayerNode")) {
			DreamCharacter* player_from_group;
			player_from_group = static_cast<DreamCharacter *>(get_tree()->get_first_node_in_group("PlayerNode"));
			if (player_from_group != nullptr) {
				player_node = player_from_group;
				return player_from_group;
			}	
		}
	}
	return nullptr;
}

void DreamScene::set_player(DreamCharacter* p_player) {
	player_node = p_player;
}

#endif