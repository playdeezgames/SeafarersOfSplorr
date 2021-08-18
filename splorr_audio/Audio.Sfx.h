#pragma once
#include <functional>
#include <optional>
#include <string>
namespace audio::Sfx
{
	void SetStore(int);
	template<typename TStore>
	std::function<void()> DoSetStore(const TStore& store)
	{
		return [store]() 
		{
			SetStore((int)store);
		};
	}
	void Play(const std::optional<std::string>&);
	void SetVolume(int);
	int GetVolume();
}


