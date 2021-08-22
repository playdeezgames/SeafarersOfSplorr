#pragma once
#include <Common.XY.h>
#include <functional>
namespace application::MouseMotion
{
	typedef std::function<void(const common::XY<int>&)> Handler;
	void AddHandler(int, Handler);
	void Handle(const int&, const int&);
	template<typename TState>
	void AddHandler(const TState& state, Handler handler)
	{
		AddHandler((int)state, handler);
	}
}
