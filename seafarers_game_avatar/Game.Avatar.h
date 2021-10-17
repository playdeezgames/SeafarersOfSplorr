#pragma once
#include "Game.Avatar.Action.h"
#include "Game.Avatar.State.h"
#include "Game.Difficulty.h"
#include "Game.Item.h"
#include <optional>
#include <string>
namespace game//20211017
{
	struct Avatar
	{
		static bool DoAction(const avatar::Action&);
		static std::optional<avatar::State> GetState();
		static void ApplyTurnEffects();
		static void Reset(const Difficulty&);
		static std::optional<std::string> GetName(int);
	};
}