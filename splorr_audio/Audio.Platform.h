#pragma once
#include <string>
namespace audio
{
	struct Platform
	{
		static const int VOLUME_MAXIMUM;
		static const int VOLUME_MINIMUM;
		static void PauseMusic();
		static void ResumeMusic();
		static size_t LoadMusic(const std::string&);
		static void UnloadMusic(size_t);
		static void SetMusicVolumne(int);
		static int GetMusicVolume();
		static void StartMusic(size_t);
	};
}
