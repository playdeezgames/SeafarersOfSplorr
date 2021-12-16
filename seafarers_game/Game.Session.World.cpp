#include <Common.Heading.h>
#include <Common.RNG.h>
#include <Data.Game.World.h>
#include "Game.Session.World.h"
#include "Game.World.h"
namespace game::session
{
	static const int CURRENT_VERSION = 1;

	World::World()
	{

	}

	world::Wind World::GetWind() const
	{
		return world::Wind();
	}

	struct WorldProperties
	{
		double size;
		double minimumIslandDistance;
		double viewDistance;
		double dockDistance;
		double unfoulingLaborMultiplier;
		double reputationReward;
		double reputationPenalty;
	};

	static const std::map<game::Difficulty, WorldProperties> worldProperties =
	{
		{game::Difficulty::EASY, {100.0, 10.0, 10.0, 1.0, 100.0, 1.0, -1.0}},
		{game::Difficulty::NORMAL, {150.0, 15.0, 10.0, 1.0, 100.0, 1.0, -1.0}},
		{game::Difficulty::HARD, {200.0, 20.0, 10.0, 1.0, 100.0, 1.0, -1.0}},
		{game::Difficulty::HARDCORE, {250.0, 25.0, 10.0, 1.0, 100.0, 1.0, -1.0}}
	};

	void World::Reset(const Difficulty& difficulty) const
	{
		auto properties = worldProperties.find(difficulty)->second;
		data::game::World data =
		{
			CURRENT_VERSION,
			{properties.size, properties.size},
			properties.minimumIslandDistance,
			properties.viewDistance,
			properties.dockDistance,
			common::RNG::FromRange(0.0, common::Heading::DEGREES),
			properties.unfoulingLaborMultiplier,
			properties.reputationReward,
			properties.reputationPenalty
		};
		data::game::World::Write(data);
	}

	void World::ApplyTurnEffects() const
	{
		GetWind().ApplyTurnEffects();
	}

	world::Bounds World::GetBounds() const
	{
		return world::Bounds();
	}

}