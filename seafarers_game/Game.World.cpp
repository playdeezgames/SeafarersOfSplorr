#include <Common.Heading.h>
#include <Common.RNG.h>
#include <Data.Game.World.h>
#include "Game.World.h"
#include <map>
namespace game
{
	static const int CURRENT_VERSION = 1;

	common::XY<double> World::GetSize()
	{
		return data::game::World::Read().value().size;
	}

	int World::GetVersion()
	{
		return data::game::World::Read().value().version;
	}

	double World::GetMinimumIslandDistance()
	{
		return data::game::World::Read().value().minimumIslandDistance;

	}

	double World::GetViewDistance()
	{
		return data::game::World::Read().value().viewDistance;
	}

	double World::GetDockDistance()
	{
		return data::game::World::Read().value().dockDistance;
	}

	struct WorldProperties
	{
		double size;
		double minimumIslandDistance;
		double viewDistance;
		double dockDistance;
	};

	static const std::map<game::Difficulty, WorldProperties> worldProperties =
	{
		{game::Difficulty::EASY, {100.0, 10.0, 10.0, 1.0}},
		{game::Difficulty::NORMAL, {150.0, 15.0, 10.0, 1.0}},
		{game::Difficulty::HARD, {200.0, 20.0, 10.0, 1.0}},
		{game::Difficulty::HARDCORE, {250.0, 25.0, 10.0, 1.0}}
	};

	void World::Reset(const game::Difficulty& difficulty)
	{
		auto properties = worldProperties.find(difficulty)->second;
		data::game::World data = 
		{
			CURRENT_VERSION,
			{properties.size, properties.size},
			properties.minimumIslandDistance,
			properties.viewDistance,
			properties.dockDistance,
			common::RNG::FromRange(0.0, common::Heading::DEGREES)
		};
		data::game::World::Write(data);
	}

	double World::GetWindHeading()
	{
		return data::game::World::Read().value().windHeading;
	}

	void World::SetWindHeading(double heading)
	{
		auto data = data::game::World::Read();
		if (data)
		{
			data.value().windHeading = heading;
			data::game::World::Write(data.value());
		}
	}

	double World::GetWindSpeedMultiplier(double heading)
	{
		auto relativeHeading = common::Heading::Difference(game::World::GetWindHeading(), heading);
		return 1.0 - std::abs(relativeHeading / common::Heading::DEGREES);
	}

	double World::GetUnfoulingLaborMultiplier()
	{
		return 100.0;//TODO: put this in configuration somewhere!
	}

}