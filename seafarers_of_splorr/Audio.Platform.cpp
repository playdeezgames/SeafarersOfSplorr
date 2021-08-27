#include <Audio.Platform.h>
#include <SDL_mixer.h>
#include <vector>
namespace audio
{
	static std::vector<Mix_Music*> music;
	static const int LOOP_FOREVER = -1;
	static const int FADE_IN_MILLISECONDS = 1000;

	const int Platform::VOLUME_MAXIMUM = MIX_MAX_VOLUME;

	const int Platform::VOLUME_MINIMUM = 0;


	void Platform::PauseMusic()
	{
		Mix_PauseMusic();
	}

	void Platform::ResumeMusic()
	{
		Mix_ResumeMusic();
	}

	size_t Platform::LoadMusic(const std::string& filename)
	{
		auto result = music.size();
		music.push_back(Mix_LoadMUS(filename.c_str()));
		return result;
	}

	void Platform::UnloadMusic(size_t index)
	{
		if (music[index])
		{
			Mix_FreeMusic(music[index]);
			music[index] = nullptr;
		}
	}

	void Platform::SetMusicVolumne(int volume)
	{
		Mix_VolumeMusic(volume);
	}

	int Platform::GetMusicVolume()
	{
		return Mix_VolumeMusic(-1);
	}

	void Platform::StartMusic(size_t index)
	{
		Mix_FadeInMusic(music[index], LOOP_FOREVER, FADE_IN_MILLISECONDS);
	}
}