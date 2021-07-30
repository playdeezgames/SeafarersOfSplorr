#pragma once
#include "Common.XY.h"
#include <functional>
#include "UIState.h"
namespace application::MouseMotion
{
	typedef std::function<void(const common::XY<int>&)> Handler;
	void AddHandler(const ::UIState&, Handler);
	void Handle(const int&, const int&);
}
