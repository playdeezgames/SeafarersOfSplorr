#pragma once
#include "Game.Fish.h"
#include <optional>
namespace game
{
	struct FishboardCell
	{
		bool revealed;
		std::optional<Fish> fish;
	};
}