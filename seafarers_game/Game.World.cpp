#include <Common.Data.h>
#include <Common.Heading.h>
#include <Common.RNG.h>
#include <Data.Game.World.h>
#include "Game.World.h"
#include <map>
namespace game
{
	static const int CURRENT_VERSION = 1;

	static data::game::World Read()
	{
		return data::game::World::Read().value();
	}

	common::XY<double> World::GetSize()
	{
		return Read().size;
	}

	int World::GetVersion()
	{
		return Read().version;
	}

	double World::GetMinimumIslandDistance()
	{
		return Read().minimumIslandDistance;
	}

	double World::GetViewDistance()
	{
		return Read().viewDistance;
	}

	double World::GetDockDistance()
	{
		return Read().dockDistance;
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
			common::RNG::FromRange(0.0, common::Heading::DEGREES),
			properties.unfoulingLaborMultiplier,
			properties.reputationReward,
			properties.reputationPenalty
		};
		data::game::World::Write(data);
	}

	double World::GetWindHeading()
	{
		return Read().windHeading;
	}

	void World::SetWindHeading(double heading)
	{
		auto data = data::game::World::Read();
		if (data)
		{
			data.value().windHeading = common::Data::ModuloDouble(heading, common::Heading::DEGREES).value();
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
		return Read().unfoulingLaborMultiplier;
	}

	static void ApplyWindChange()
	{
		const double NORMAL_WIND_CHANGE = 5.0;
		game::World::SetWindHeading(game::World::GetWindHeading() + common::RNG::FromRange(-NORMAL_WIND_CHANGE, NORMAL_WIND_CHANGE));
	}

	void World::ApplyTurnEffects()
	{
		ApplyWindChange();
	}

	bool World::ClampLocation(common::XY<double>& location)
	{
		bool result = false;
		auto worldSize = game::World::GetSize();
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

	double World::GetReputationReward()
	{
		return 1.0;
	}

	double World::GetReputationPenalty()
	{
		return -1.0;
	}

}