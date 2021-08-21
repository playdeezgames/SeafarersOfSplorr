#pragma once
#include <functional>
#include <string>
namespace application::Keyboard
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
	typedef std::function<bool(const std::string&)> Handler;
	void AddHandler(int, Handler);
	bool Handle(const std::string&);
	template<typename TState>
	void AddHandler(const TState& state, Handler handler)
	{
		AddHandler((int)state, handler);
	}
}
