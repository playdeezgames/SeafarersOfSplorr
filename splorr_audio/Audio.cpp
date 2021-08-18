#include <SDL_mixer.h>
namespace audio::Sfx
{
	void Initialize();
}
namespace audio::Mux
{
	void Initialize();
}
namespace Audio
{
	int ClampVolume(int volume)
	{
		return
			(volume < 0) ? (0) :
			(volume > MIX_MAX_VOLUME) ? (MIX_MAX_VOLUME) :
			(volume);
	}

	static bool muted = false;

	void Initialize()
	{
		::audio::Sfx::Initialize();
		::audio::Mux::Initialize();
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
