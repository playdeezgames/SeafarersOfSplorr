#pragma once
#include "Command.h"
#include <functional>
#include <map>
#include "UIState.h"
namespace application::Command
{
	void SetHandlers(const ::UIState&, const std::map<::Command, std::function<void()>>&);
	void SetHandler(const ::UIState&, std::function<void()>);
}
