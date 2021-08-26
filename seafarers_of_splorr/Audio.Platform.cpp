#include <Audio.Platform.h>
#include <SDL_mixer.h>
namespace audio
{
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
}