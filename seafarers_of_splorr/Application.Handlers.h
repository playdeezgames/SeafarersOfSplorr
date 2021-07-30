#pragma once
#include "Application.UIState.h"
#include <map>
namespace application::Handlers
{
	template<typename THandler, typename TAction>
	void WithCurrent(const std::map<::UIState, THandler> handlers, TAction action)
	{
		auto iter = handlers.find(application::UIState::Read());
		if (iter != handlers.end())
		{
			action(iter->second);
		}
	}
}