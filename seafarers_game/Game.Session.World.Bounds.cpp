#include <Common.Utility.Table.h>
#include <Data.Game.World.h>
#include "Game.Difficulty.h"
#include "Game.Session.World.Bounds.h"
namespace game::session::world
{
	using WorldData = data::game::World;

	common::XY<double> Bounds::GetSize() const
	{
		auto difficulty = (Difficulty)WorldData::ReadDifficulty(worldId).value();
		return
			(difficulty == Difficulty::EASY) ? (common::XY<double>(100.0, 100.0)) :
			(difficulty == Difficulty::NORMAL) ? (common::XY<double>(150.0, 150.0)) :
			(difficulty == Difficulty::HARD) ? (common::XY<double>(200.0, 200.0)) :
			(common::XY<double>(250.0, 250.0));
	}

	bool Bounds::ClampLocation(common::XY<double>& location) const
	{
		bool result = false;
		auto worldSize = GetSize();
		if (location.GetX() < 0.0)
		{
			result = true;
			location = { 0, location.GetY() };
		}
		if (location.GetX() > worldSize.GetX())
		{
			result = true;
			location = { worldSize.GetX(), location.GetY() };
		}
		if (location.GetY() < 0.0)
		{
			result = true;
			location = { location.GetX(), 0.0 };
		}
		if (location.GetY() > worldSize.GetY())
		{
			result = true;
			location = { location.GetX(), worldSize.GetY() };
		}
		return result;
	}
}