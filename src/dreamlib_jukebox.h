#if 0
//#ifndef DREAMLIB_JUKEBOX_H
//#define DREAMLIB_JUKEBOX_H

#include <godot_cpp/classes/audio_stream.hpp>
#include <godot_cpp/classes/audio_stream_player.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/tween.hpp>
#include <godot_cpp/classes/property_tweener.hpp>


namespace godot {
class DreamJukebox : public Node {
	GDCLASS(DreamJukebox, Node)

private:
	const double BGM_VOLUME_MIN_DB = -40.0;
	const Callable bgm_queue_finished = Callable(this, "bgm_play");
	const Callable bgm_transition_finished = Callable(this, "bgm_trans_finished");
	const Callable bgm_stop_signal = Callable(this, "bgm_kill");
	String bgm_path;

	AudioStreamPlayer *stream_1;
	AudioStreamPlayer *stream_2;

	Ref<Tween> tween_stream_1;
	Ref<Tween> tween_stream_2;

	ResourceLoader *bgm_loader;

	struct {
		String bgm_name;
		double trans_sec;
		double bgm_pitch;
		double vol;
		AudioStreamPlayer *stream_player;
	} now_playing;
	

protected:
	static void _bind_methods();

public:
	DreamJukebox();
	~DreamJukebox();

	void bgm_play(String bgm, double transition_seconds, double pitch, double volume);
	void bgm_stop(double transition_seconds = 2.0);

	void set_stream_player_1(AudioStreamPlayer* p_value);
	void set_stream_player_2(AudioStreamPlayer* p_value);
	AudioStreamPlayer* get_stream_player_1() const;
	AudioStreamPlayer* get_stream_player_2() const;

	void bgm_trans_finished();
	void set_bgm_path(String p_value);
	void bgm_kill();
	String get_bgm_path() const;

	Dictionary get_now_playing() const;
	
};
} //namespace godot

#endif
