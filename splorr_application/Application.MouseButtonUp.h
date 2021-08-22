#pragma once
#include <Common.XY.h>
#include <functional>
#include "MouseButton.h"
namespace application::MouseButtonUp
{
	typedef std::function<bool(const common::XY<int>&, MouseButton)> Handler;
	void AddHandler(int, Handler);
	template<typename TState>
	void AddHandler(const TState& state, Handler handler)
	{
		AddHandler((int)state, handler);
	}
}
