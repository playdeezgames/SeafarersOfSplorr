#pragma once
#include <functional>
#include <string>
namespace application::TextInput
{
	typedef std::function<void(const std::string&)> Handler;
	void AddHandler(int, Handler);
	void Handle(const std::string&);
	template<typename TState>
	void AddHandler(const TState& state, Handler handler)
	{
		AddHandler((int)state, handler);
	}
}

