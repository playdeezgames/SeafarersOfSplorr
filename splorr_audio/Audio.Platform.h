#pragma once
namespace audio
{
	struct Platform
	{
		static const int VOLUME_MAXIMUM;
		static const int VOLUME_MINIMUM;
		static void PauseMusic();
		static void ResumeMusic();
	};
}
