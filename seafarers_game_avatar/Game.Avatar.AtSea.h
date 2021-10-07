#pragma once
#include <Common.XY.h>
#include "Game.Difficulty.h"
#include <optional>
namespace game::avatar
{
	struct AtSea
	{
		enum class MoveResult//TODO: move to ship
		{
			MOVED,
			CLAMPED
		};

		static MoveResult Move();//TODO: move to ship
	};
}
