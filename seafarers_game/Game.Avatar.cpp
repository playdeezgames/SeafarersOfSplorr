#include <Data.Game.Avatar.h>
#include <functional>
#include "Game.Avatar.h"
#include "Game.Avatar.Log.h"
#include "Game.Avatar.StateTransition.h"
#include <map>
namespace game::avatar
{
	static void SetState(const game::avatar::State& state)
	{
		data::game::Avatar::SetState((int)state);
	}

	const std::map<avatar::Action, std::map<avatar::State, std::function<StateTransition()>>>& GetActionDescriptors();

	bool DoAction(const avatar::Action& action)
	{
		auto state = GetState();
		if (state)
		{
			auto descriptor = GetActionDescriptors().find(action);
			if (descriptor != GetActionDescriptors().end())
			{
				auto transition = descriptor->second.find(state.value());
				{
					if (transition != descriptor->second.end())
					{
						auto result = transition->second();
						avatar::Log::Write({ result.logColor, result.logText });
						SetState(result.dockedState);
					}
				}
			}
		}
		return false;
	}

	std::optional<game::avatar::State> GetState()
	{
		auto state = data::game::Avatar::GetState();
		if (state)
		{
			return (game::avatar::State)state.value();
		}
		return std::nullopt;
	}
}