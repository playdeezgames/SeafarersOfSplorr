#include <Data.Game.Avatar.Dock.h>
#include <functional>
#include "Game.Avatar.h"
#include "Game.Avatar.Docked.h"
#include "Game.Avatar.Log.h"
#include "Game.Avatar.StateTransition.h"
#include <map>
namespace game::avatar::Docked
{
	bool Undock(const avatar::Action&);
}
namespace game::avatar
{
	static bool SetDockedState(const game::avatar::State& dockedState)
	{
		auto location = game::avatar::Docked::GetDockedLocation();
		if (location)
		{
			data::game::avatar::Dock::SetLocation(location.value(), (int)dockedState);
		}
		return false;
	}

	const std::map<avatar::Action, std::map<avatar::State, std::function<StateTransition()>>>& GetActionDescriptors();

	bool DoActionTransition(const avatar::Action& action)
	{
		auto dockedState = GetState();
		if (dockedState)
		{
			auto descriptor = GetActionDescriptors().find(action);
			if (descriptor != GetActionDescriptors().end())
			{
				auto transition = descriptor->second.find(dockedState.value());
				{
					if (transition != descriptor->second.end())
					{
						auto result = transition->second();
						avatar::Log::Write({ result.logColor, result.logText });
						SetDockedState(result.dockedState);
					}
				}
			}
		}
		return false;
	}

	const std::map<avatar::Action, std::function<bool(const avatar::Action&)>> dockedActions =
	{
		{ avatar::Action::UNDOCK, game::avatar::Docked::Undock}
	};

	bool DoAction(const avatar::Action& action)
	{
		auto iter = dockedActions.find(action);
		if (iter != dockedActions.end())
		{
			return iter->second(action);
		}
		return DoActionTransition(action);
	}

	std::optional<game::avatar::State> GetState()
	{
		auto state = data::game::avatar::Dock::GetState();
		if (state)
		{
			return (game::avatar::State)state.value();
		}
		return std::nullopt;
	}
}