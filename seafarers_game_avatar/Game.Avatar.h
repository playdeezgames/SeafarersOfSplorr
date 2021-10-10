#pragma once
#include "Game.Avatar.Action.h"
#include "Game.Avatar.State.h"
#include "Game.Difficulty.h"
#include "Game.Item.h"
#include <optional>
#include <string>
namespace game
{
	struct Avatar
	{
		static bool DoAction(const avatar::Action&);
		static std::optional<game::avatar::State> GetState();
		static void ApplyTurnEffects();
		static void Reset(const game::Difficulty&);
		static std::optional<game::Item> GetRations();
		static void SetRations(const std::optional<game::Item>&);
		static std::optional<std::string> GetName(int);
	};
}
