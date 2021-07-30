#pragma once
#include "Common.XY.h"
#include "Application.UIState.h"
#include <functional>
enum class MouseButton
{
	LEFT,
	MIDDLE,
	RIGHT,
	X1,
	X2
};
namespace application::MouseButtonUp
{
	typedef std::function<bool(const common::XY<int>&, MouseButton)> Handler;
	void AddHandler(const ::UIState&, Handler);
	void Handle(const int&, const int&, const MouseButton&);
}
