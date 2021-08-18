#pragma once
#include <functional>
namespace application::OnEnter
{
	typedef std::function<void()> Handler;
	void AddHandler(int, Handler);
	template<typename TState>
	void AddHandler(const TState& state, Handler handler)
	{
		AddHandler((int)state, handler);
	}
}