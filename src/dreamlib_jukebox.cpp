#if 0

#include "dreamlib_jukebox.h"
#include "gde_util.h"

#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void DreamJukebox::_bind_methods() {

	//ClassDB::bind_method(D_METHOD("test"), &DreamJukebox::test);

	ClassDB::bind_method(D_METHOD("bgm_play", "bgm_file", "transition_seconds", "pitch", "volume"), &DreamJukebox::bgm_play);
	ClassDB::bind_method(D_METHOD("bgm_stop", "transition_seconds"), &DreamJukebox::bgm_stop);
	ClassDB::bind_method(D_METHOD("get_now_playing"), &DreamJukebox::get_now_playing);
	EXPORT_PROPERTY(STRING, bgm_path, DreamJukebox);
	
	ClassDB::bind_method(D_METHOD("set_" "stream_player_1", "p_value"), &DreamJukebox::set_stream_player_1);
	ClassDB::bind_method(D_METHOD("get_" "stream_player_1"), &DreamJukebox::get_stream_player_1);
	ClassDB::add_property(get_class_static(), PropertyInfo(Variant::OBJECT, "stream_player_1", PROPERTY_HINT_NODE_TYPE, "AudioStreamPlayer"), "set_" "stream_player_1", "get_" "stream_player_1");
	
	ClassDB::bind_method(D_METHOD("set_" "stream_player_2", "p_value"), &DreamJukebox::set_stream_player_2); 
	ClassDB::bind_method(D_METHOD("get_" "stream_player_2"), &DreamJukebox::get_stream_player_2); 
	ClassDB::add_property(get_class_static(), PropertyInfo(Variant::OBJECT, "stream_player_2", PROPERTY_HINT_NODE_TYPE, "AudioStreamPlayer"), "set_" "stream_player_2", "get_" "stream_player_2");
}

DreamJukebox::DreamJukebox() {
	bgm_path = "res://asset/bgm/";
	stream_1 = nullptr;
	stream_2 = nullptr;

	now_playing.bgm_name = "";
	now_playing.bgm_pitch = 1.0;
	now_playing.trans_sec = 2.0;
	now_playing.vol = -0.3;
}

DreamJukebox::~DreamJukebox() {}

void DreamJukebox::bgm_play(String bgm, double transition_seconds = 2.0, double pitch = 1.0, double volume = -0.3) {
	if (bgm.is_empty()) {
		bgm_stop(transition_seconds);
		return;
	}
	AudioStreamPlayer *available_player;
	AudioStreamPlayer *not_available_player;
	if (stream_1->is_playing() && stream_2->is_playing()) {
		if (now_playing.stream_player == stream_1) {
			available_player = stream_2;
			not_available_player = stream_1;
		} else {
			available_player = stream_1;
			not_available_player = stream_2;
		}
	} else if (stream_1->is_playing()) {
		available_player = stream_2;
		not_available_player = stream_1;
	} else if (stream_2->is_playing()) {
		available_player = stream_1;
		not_available_player = stream_2;
	} else {
		now_playing.bgm_name = "";
		available_player = stream_1;
		not_available_player = stream_2;
	}
	String bgm_file;
	if (!bgm.is_absolute_path()) {
		bgm_file = (bgm_path + bgm.to_snake_case()).validate_filename();
		if (bgm_loader->exists(bgm_file + ".ogg")) {
			bgm_file = bgm_file + ".ogg";
		} else if (bgm_loader->exists(bgm_file + ".wav")) {
			bgm_file = bgm_file + ".wav";
		} else if (bgm_loader->exists(bgm_file + ".res")) {
			bgm_file = bgm_file + ".res";
		} else if (bgm_loader->exists(bgm_file + ".tres")) {
			bgm_file = bgm_file + ".tres";
		} else if (bgm_loader->exists(bgm_file + ".mp3")) {
			bgm_file = bgm_file + ".mp3";
		} else {
			print_error("BGM file ", bgm_file, "does not exist");
			bgm_stop(transition_seconds);
			return;
		}
	} else {
		bgm_file = bgm;
	}
	if (now_playing.bgm_name == bgm_file) {
		if (now_playing.bgm_pitch == pitch) {
			return;
		}
	} else {
		now_playing.bgm_name = bgm_file;
		now_playing.trans_sec = transition_seconds;
		now_playing.bgm_pitch = pitch;
		now_playing.vol = volume;
		now_playing.stream_player = available_player;
	}

/* 	if (tween_stream_1.is_valid()) {
		tween_stream_1->disconnect("finished", bgm_transition_finished);
		tween_stream_1->kill();
	}
	if (tween_stream_2.is_valid()) {
		tween_stream_2->disconnect("finished", bgm_transition_finished);
		tween_stream_2->kill();
	} */
	available_player->set_stream(bgm_loader->load(bgm_file, "AudioStream"));
	available_player->set_volume_db(BGM_VOLUME_MIN_DB);
	available_player->set_pitch_scale(pitch);
	available_player->play();

	//tween_stream_1 = get_tree()->create_tween(); //set_pause_mode(Tween::TWEEN_PAUSE_PROCESS);
	//tween_stream_2 = get_tree()->create_tween(); //set_pause_mode(Tween::TWEEN_PAUSE_PROCESS);
	tween_stream_1 = get_tree()->create_tween()->tween_property(available_player, "volume_db", volume, transition_seconds);
	tween_stream_2->connect("finished", bgm_transition_finished, ConnectFlags::CONNECT_ONE_SHOT);
	tween_stream_2 = get_tree()->create_tween()->tween_property(not_available_player, "volume_db", BGM_VOLUME_MIN_DB, transition_seconds);
}

void DreamJukebox::bgm_trans_finished() {
	if (Math::is_equal_approx(stream_1->get_volume_db(), float(BGM_VOLUME_MIN_DB))) {
		stream_1->stop();
	}
	if (Math::is_equal_approx(stream_2->get_volume_db(), float(BGM_VOLUME_MIN_DB))) {
		stream_2->stop();
	}
}

void DreamJukebox::set_bgm_path(String p_value) {
	bgm_path = p_value;
}


String DreamJukebox::get_bgm_path() const {
	return bgm_path;
}

void DreamJukebox::bgm_stop(double transition_seconds) {
	if (tween_stream_1.is_valid()) {
		tween_stream_1->kill();
	}
	if (tween_stream_2.is_valid()) {
		tween_stream_2->kill();
	}
	if (stream_1->is_playing()) {
		Ref<Tween> tween;
		tween = get_tree()->create_tween()->set_pause_mode(Tween::TWEEN_PAUSE_PROCESS);
		tween->tween_property(stream_1, "volume_db", BGM_VOLUME_MIN_DB, transition_seconds);
		tween->connect("finished", bgm_stop_signal, ConnectFlags::CONNECT_ONE_SHOT);
	}
	if (stream_2->is_playing()) {
		Ref<Tween> tween;
		tween = get_tree()->create_tween()->set_pause_mode(Tween::TWEEN_PAUSE_PROCESS);
		tween->tween_property(stream_2, "volume_db", BGM_VOLUME_MIN_DB, transition_seconds);
		tween->connect("finished", bgm_stop_signal, ConnectFlags::CONNECT_ONE_SHOT);
	}
}

void godot::DreamJukebox::set_stream_player_1(AudioStreamPlayer *p_value) {
	stream_1 = p_value;
}

void godot::DreamJukebox::set_stream_player_2(AudioStreamPlayer *p_value) {
	stream_2 = p_value;
}

AudioStreamPlayer *godot::DreamJukebox::get_stream_player_1() const {
	return stream_1;
}

AudioStreamPlayer *godot::DreamJukebox::get_stream_player_2() const {
	return stream_2;
}

void DreamJukebox::bgm_kill() {
	stream_1->stop();
	stream_2->stop();
	now_playing.bgm_name = "";
}


Dictionary DreamJukebox::get_now_playing() const {
	Dictionary dict;
	dict.set("bgm_name", now_playing.bgm_name);
	dict.set("trans_sec", now_playing.trans_sec);
	dict.set("bgm_pitch", now_playing.bgm_pitch);
	dict.set("vol", now_playing.vol);
	dict.set("stream_player", now_playing.stream_player);
	return dict;
}

/* void DreamJukebox::test(){
	//Time tim = Time->get_singleton();
	begin = Time::get_singleton()->get_ticks_usec();

	for (int i = 0; i < 1; i++) {
		String text = "hello" + String::num_int64(i);
		if (text.is_empty()){
			text = "asd";
		}
		text += get_tree_string();
		Array arr;
		arr.append(text);
		if (!text.is_empty()) {
			Dictionary dic;
			dic.set(StringName("kasd"), Math::sin(3.0));
			double eeeasd = double(dic.get("kasd", 1.0)) + 1234.0 * 2.0;
			dic.set(eeeasd, eeeasd);
			eeeasd += double(dic.find_key(eeeasd));
		}
	}
	end = Time::get_singleton()->get_ticks_usec();
	print_line(end - begin);

} */

#endif