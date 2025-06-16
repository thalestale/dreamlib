#include "register_types.h"

#include "dreamlib_camera.h"
#include "dreamlib_character.h"
//#include "dreamlib_item.h"
//#include "dreamlib_scene.h"
#include "dreamlib_scene_changer.h"
//#include "dreamlib_jukebox.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

using namespace godot;

void initialize_dreamlib_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	GDREGISTER_CLASS(DreamCharacter);
	GDREGISTER_CLASS(DreamCamera);
	//GDREGISTER_CLASS(DreamItem);
	//GDREGISTER_CLASS(DreamScene);
	GDREGISTER_CLASS(DreamSceneChanger);
	//GDREGISTER_CLASS(DreamJukebox);
	
}

void uninitialize_dreamlib_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

extern "C" {
	// Initialization.
	GDExtensionBool GDE_EXPORT dreamlib_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization* r_initialization) {
		godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

		init_obj.register_initializer(initialize_dreamlib_module);
		init_obj.register_terminator(uninitialize_dreamlib_module);
		init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

		return init_obj.init();
	}
}