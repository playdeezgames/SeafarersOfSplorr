#include <Data.Game.World.h>
#include "Game.Difficulty.h"
#include "Game.Session.World.Distances.h"
#include <map>
namespace game::session::world
{
	using WorldData = data::game::World;

	static const std::map<Difficulty, double> islandDistancesByDifficulty =
	{
		{ Difficulty::EASY, 10.0 },
		{ Difficulty::NORMAL, 15.0 },
		{ Difficulty::HARD, 20.0 },
		{ Difficulty::HARDCORE, 25.0 },
	};

	double Distances::GetIslandSpacing() const
	{
		return islandDistancesByDifficulty.find((Difficulty)WorldData::ReadDifficulty(worldId).value())->second;
	}
}