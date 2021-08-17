#pragma once
#include <optional>
#include <string>
namespace audio::Sfx
{
	void Play(const std::optional<std::string>&);
	void SetVolume(int);
	int GetVolume();
}


