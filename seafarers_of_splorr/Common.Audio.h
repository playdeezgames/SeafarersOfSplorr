#pragma once
#include <map>
#include <string>
#include <optional>
namespace common::audio
{
	namespace Sfx
	{
		void Play(const std::optional<std::string>&);
		void SetVolume(int);
		int GetVolume();
	}
}
namespace common::audio
{
	namespace Mux
	{
		void Play(const std::string&);
		void SetVolume(int);
		int GetVolume();
	}
}
