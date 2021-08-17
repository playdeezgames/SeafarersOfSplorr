#include "Audio.h"
#include "Data.JSON.Stores.h"
#include "json.hpp"
#include <map>
#include <memory>
#include <optional>
#include <SDL_mixer.h>
#include <string>
namespace Audio
{
	int ClampVolume(int);
}
namespace audio::Mux
{
	static std::map<std::string, std::shared_ptr<Mix_Music>> music;
	static int muxVolume = MIX_MAX_VOLUME;

	const int LOOP_FOREVER = -1;
	static void HookFreeMusic(Mix_Music* m)
	{
		Mix_FreeMusic(m);
	}

	static void AddMusic(const std::string& name, const std::string& filename)
	{
		music[name] = std::shared_ptr<Mix_Music>(Mix_LoadMUS(filename.c_str()), HookFreeMusic);
	}

	static void Finish()
	{
		while (music.begin() != music.end())
		{
			music.erase(music.begin());
		}
	}

	static bool initialized = false;

	void Initialize()
	{
		if (!initialized)
		{
			atexit(Finish);
			nlohmann::json& j = data::json::Stores::GetStore(data::json::Store::MUSIC_THEMES);
			for (auto& i : j.items())
			{
				Mux::AddMusic(i.key(), i.value());
			}
			initialized = true;
		}
	}

	static std::optional<std::string> currentSong = std::nullopt;

	void Play(const std::string& song)
	{
		Initialize();
		if (!Audio::IsMuted())
		{
			if (!currentSong.has_value() || currentSong.value() != song)
			{
				currentSong = song;
				const auto& item = music.find(song);
				Mix_FadeInMusic(item->second.get(), LOOP_FOREVER, 1000);
			}
		}
	}

	void SetVolume(int volume)
	{
		Initialize();
		muxVolume = Audio::ClampVolume(volume);
		Mix_VolumeMusic(muxVolume);
	}

	int GetVolume()
	{
		return muxVolume;
	}

}

