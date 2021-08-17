#include <SDL_mixer.h>
namespace common::audio::Sfx
{
	void Initialize();
}
namespace common::audio::Mux
{
	void Initialize();
}
namespace Audio
{
	static bool muted = false;

	void Initialize()
	{
		common::audio::Sfx::Initialize();
		common::audio::Mux::Initialize();
	}

	void SetMuted(bool newValue)
	{
		Initialize();
		muted = newValue;
		if (muted)
		{
			Mix_PauseMusic();
		}
		else
		{
			Mix_ResumeMusic();
		}
	}

	bool IsMuted()
	{
		return muted;
	}
}
