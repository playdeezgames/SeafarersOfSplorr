#pragma once
#include <optional>
#include <string>
#include <functional>
#include "UIState.h"
namespace application::UIState
{
	void Write(const ::UIState&);
	const ::UIState& Read();
	std::function<void()> GoTo(const ::UIState&);

	void Push(const ::UIState&);
	void Pop();
	std::function<void()> PushTo(const ::UIState&);
	std::function<void()> PopFrom();
}