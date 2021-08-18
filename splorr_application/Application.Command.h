#pragma once
#include "Command.h"
#include <functional>
#include <map>
namespace application::Command
{
	void SetHandlers(int, const std::map<::Command, std::function<void()>>&);
	template<typename TState>
	void SetHandlers(const TState& state, const std::map<::Command, std::function<void()>>& handlers)
	{
		SetHandlers((int)state, handlers);
	}
	void SetHandler(int, std::function<void()>);
	template<typename TState>
	void SetHandler(const TState& state, std::function<void()> handler)
	{
		SetHandler((int)state, handler);
	}
}
