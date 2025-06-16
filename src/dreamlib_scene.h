#if 0
//#ifndef DREAMLIB_SCENE_H
//#define DREAMLIB_SCENE_H

#include "dreamlib_character.h"
#include <godot_cpp/classes/node.hpp>

namespace godot {
	class DreamScene : public Node {
		GDCLASS(DreamScene, Node)

	private:

		String scene_name;

		String default_bgm_file_name;
		double default_bgm_volume;
		double default_bgm_fade;

		String default_ambience_file_name;
		double default_ambience_volume;
		double default_ambience_fade;

		bool inventory_key_permited;

		DreamCharacter *player_node;

	protected:
		static void _bind_methods();
	public:
		DreamScene();
		~DreamScene();

		String get_scene_name() const;
		bool is_inventory_key_permited() const;

		void set_scene_name(String p_name_value);	
		void set_inventory_key_permited(bool p_enable_value);

		void set_default_bgm_file_name(String p_name_value);
		void set_default_bgm_volume(double p_volume);
		void set_default_bgm_fade(double p_fade);
		double get_default_bgm_volume() const;
		double get_default_bgm_fade() const;
		String get_default_bgm_file_name() const;

		void set_default_ambience_file_name(String p_name_value);
		void set_default_ambience_volume(double p_volume);
		void set_default_ambience_fade(double p_fade);
		double get_default_ambience_volume() const;
		double get_default_ambience_fade() const;
		String get_default_ambience_file_name() const;

		DreamCharacter* get_player();
		void set_player(DreamCharacter* p_player);

		//void _notification(int p_what);

	};
}
#endif