#pragma once
#include <functional>
namespace application::Update
{
	typedef std::function<void(const unsigned int&)> Handler;
	void AddHandler(int, Handler);
	template<typename TState>
	void AddHandler(const TState& state, Handler handler)
	{
		AddHandler((int)state, handler);
	}
}