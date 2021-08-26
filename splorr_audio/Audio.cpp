#include <SDL_mixer.h>
#include "Audio.Mux.h"
#include "Audio.Sfx.h"
#include "Audio.h"
int Audio::ClampVolume(int volume)
{
	return
		(volume < 0) ? (0) :
		(volume > MIX_MAX_VOLUME) ? (MIX_MAX_VOLUME) :
		(volume);
}

static bool muted = false;

static void Initialize()
{
	::audio::Sfx::Initialize();
	::audio::Mux::Initialize();
}

void Audio::SetMuted(bool newValue)
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

bool Audio::IsMuted()
{
	return muted;
}
