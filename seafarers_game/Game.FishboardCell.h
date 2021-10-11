#pragma once
#include "Game.Fish.h"
#include <optional>
namespace game//20211011
{
	struct FishboardCell
	{
		bool revealed;
		std::optional<Fish> fish;
	};
}