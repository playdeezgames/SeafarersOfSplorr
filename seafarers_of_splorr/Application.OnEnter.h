#pragma once
#include <functional>
#include "UIState.h"
namespace application::OnEnter
{
	typedef std::function<void()> Handler;
	void AddHandler(const ::UIState&, Handler);
}