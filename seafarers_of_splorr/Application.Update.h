#pragma once
#include <functional>
#include "UIState.h"
namespace application::Update
{
	typedef std::function<void(const unsigned int&)> Handler;
	void AddHandler(const ::UIState&, Handler);
}