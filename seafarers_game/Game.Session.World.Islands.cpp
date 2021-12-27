#include <Data.Game.Island.h>
#include "Game.Islands.h"
#include "Game.Islands.Features.h"
#include "Game.Session.World.Islands.h"
namespace game::session::world
{
	using IslandData = data::game::Island;

	std::list<Island> Islands::GetAll() const
	{
		auto islands = IslandData::All();
		std::list<Island> result;
		for (auto island : islands)
		{
			result.push_back(Island(island.id));
		}
		return result;
	}

	void Islands::Populate(const Difficulty& difficulty) const
	{
		game::Islands::Populate(difficulty);
		game::islands::Features::Populate(difficulty);
	}


	void Islands::Reset() const
	{
		game::Islands::Reset();
		game::islands::Features::Reset();
	}

	void Islands::ApplyTurnEffects() const
	{

	}
}