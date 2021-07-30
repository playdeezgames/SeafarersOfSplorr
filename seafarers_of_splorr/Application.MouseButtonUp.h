#pragma once
#include "Common.XY.h"
#include <functional>
#include "MouseButton.h"
#include "UIState.h"
namespace application::MouseButtonUp
{
	typedef std::function<bool(const common::XY<int>&, MouseButton)> Handler;
	void AddHandler(const ::UIState&, Handler);
}
