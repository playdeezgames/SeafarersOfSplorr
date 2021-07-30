#pragma once
#include "Common.XY.h"
#include "Application.UIState.h"
#include <functional>
#include "MouseButton.h"
namespace application::MouseButtonUp
{
	typedef std::function<bool(const common::XY<int>&, MouseButton)> Handler;
	void AddHandler(const ::UIState&, Handler);
}
