#pragma once
#include "Application.UIState.h"
#include "Command.h"
#include <functional>
#include <map>
namespace application::Command
{
	void SetHandlers(const ::UIState&, const std::map<::Command, std::function<void()>>&);
	void SetHandler(const ::UIState&, std::function<void()>);
}
