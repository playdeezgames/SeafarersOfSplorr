#pragma once
#include "Game.Avatar.Action.h"
#include "Game.Avatar.State.h"
#include <optional>
namespace game::avatar
{
	bool DoAction(const avatar::Action&);
	std::optional<game::avatar::State> GetState();
}
