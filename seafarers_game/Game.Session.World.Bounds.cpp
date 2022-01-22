#include <Common.Utility.Table.h>
#include <Data.Game.World.h>
#include "Game.Difficulty.h"
#include "Game.Session.World.Bounds.h"
#include "Game.Session.World.h"
#include <algorithm>
namespace game::session::world
{
	using WorldData = data::game::World;

	common::XY<double> Bounds::GetSize()
	{
		auto difficulty = (Difficulty)WorldData::ReadDifficulty(WORLD_ID).value();
		return
			(difficulty == Difficulty::EASY) ? (common::XY<double>(100.0, 100.0)) :
			(difficulty == Difficulty::NORMAL) ? (common::XY<double>(150.0, 150.0)) :
			(difficulty == Difficulty::HARD) ? (common::XY<double>(200.0, 200.0)) :
			(common::XY<double>(250.0, 250.0));
	}

	void Bounds::ClampLocation(common::XY<double>& location)
	{
		auto worldSize = GetSize();
		location = 
		{
			std::clamp(location.GetX(), 0.0, worldSize.GetX()),
			std::clamp(location.GetY(), 0.0, worldSize.GetY()) 
		};
	}
}