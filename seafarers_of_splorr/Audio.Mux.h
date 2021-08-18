#pragma once
#include <functional>
#include <string>
namespace audio::Mux
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
	void Play(const std::string&);
	void SetVolume(int);
	int GetVolume();
}

