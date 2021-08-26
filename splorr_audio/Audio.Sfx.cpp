#include "Audio.h"
#include "Audio.Sfx.h"
#include <Data.JSON.Stores.h>
#include <memory>
#include <json.hpp>
#include <SDL_mixer.h>
namespace audio
{
	static std::map<std::string, std::shared_ptr<Mix_Chunk>> sounds;
	static int sfxVolume = MIX_MAX_VOLUME;
	const int ANY_CHANNEL = -1;
	const int NO_LOOPS = 0;

	static void AddSound(const std::string& name, const std::string& filename)
	{
		sounds[name] = std::shared_ptr<Mix_Chunk>(Mix_LoadWAV_RW(SDL_RWFromFile(filename.c_str(), "rb"), 1), Mix_FreeChunk);
	}

	static bool initialized = false;
	static std::optional<int> store = std::nullopt;

	void Sfx::SetStore(int s)
	{
		store = s;
	}

	void Sfx::Initialize()
	{
		if (!initialized)
		{
			nlohmann::json& j = data::json::Stores::GetStore(store.value());
			for (auto& i : j.items())
			{
				AddSound(i.key(), i.value());
			}
			initialized = true;
		}
	}

	static void Play(const std::string& name)
	{
		Sfx::Initialize();
		if (!Audio::IsMuted())
		{
			const auto& item = sounds.find(name);
			if (item != sounds.end())
			{
				Mix_PlayChannel(ANY_CHANNEL, item->second.get(), NO_LOOPS);
			}
		}
	}

	void Sfx::Play(const std::optional<std::string>& name)
	{
		if (name)
		{
			Play(name.value());
		}
	}

	void Sfx::SetVolume(int volume)
	{
		Initialize();
		sfxVolume = Audio::ClampVolume(volume);
		for (auto& entry : sounds)
		{
			Mix_VolumeChunk(entry.second.get(), sfxVolume);
		}
	}

	int Sfx::GetVolume()
	{
		return sfxVolume;
	}
}
