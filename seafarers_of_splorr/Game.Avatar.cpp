#include "Game.Avatar.h"
#include "Data.Game.Avatar.h"
#include "Common.Utility.h"
#include "Application.UIState.h"
#include "Game.World.h"
#include "Common.RNG.h"
#include "Game.Heading.h"
#include "Common.Data.h"
#include "Game.Avatar.Statistics.h"
#include "Data.Game.Avatar.Dock.h"
#include "Game.Islands.h"
#include "Data.Game.Avatar.Destination.h"
#include "Game.Islands.Quests.h"
namespace game::Avatar
{
	const double SPEED_MINIMUM = 0.0;
	const double SPEED_MAXIMUM = 1.0;

	common::XY<double> GetLocation()
	{
		return data::game::Avatar::Read().value().location;
	}

	double GetHeading()
	{
		return data::game::Avatar::Read().value().heading;
	}

	void SetHeading(double heading)
	{
		auto data = data::game::Avatar::Read().value();
		data.heading = common::Data::ModuloDouble(heading, game::Heading::DEGREES);
		data::game::Avatar::Write(data);
	}

	double GetSpeed()
	{
		return data::game::Avatar::Read().value().speed;
	}


	void SetSpeed(double speed)
	{
		auto data = data::game::Avatar::Read().value();
		data.speed = common::Data::ClampDouble(speed, SPEED_MINIMUM, SPEED_MAXIMUM);
		data::game::Avatar::Write(data);
	}

	void Reset(const game::Difficulty&)
	{
		auto worldSize = game::World::GetSize();
		data::game::Avatar::AvatarData data =
			{
				{worldSize.GetX()/2.0, worldSize.GetY()/2.0},
				common::Data::ModuloDouble(common::RNG::FromRange(0.0, game::Heading::DEGREES), game::Heading::DEGREES),
				1.0
			};
		data::game::Avatar::Write(data);
	}

	static void ApplyTurnEffects()
	{
		const double TURN_DELTA = -1.0;
		const double HUNGER_DELTA = -1.0;
		const double HEALTH_DELTA = -1.0;
		game::avatar::Statistics::ChangeCurrent(game::avatar::Statistic::TURNS_REMAINING, TURN_DELTA);
		if (game::avatar::Statistics::GetCurrent(game::avatar::Statistic::HUNGER) > game::avatar::Statistics::GetMinimum(game::avatar::Statistic::HUNGER))
		{
			game::avatar::Statistics::ChangeCurrent(game::avatar::Statistic::HUNGER, HUNGER_DELTA);
		}
		else
		{
			game::avatar::Statistics::ChangeCurrent(game::avatar::Statistic::HEALTH, HEALTH_DELTA);
		}
	}

	void Move()
	{
		auto avatar = data::game::Avatar::Read().value();
		common::XY<double> delta = game::Heading::DegreesToXY(avatar.heading)  * avatar.speed;
		avatar.location = avatar.location + delta;
		ApplyTurnEffects();
		data::game::Avatar::Write(avatar);
	}

	static bool DoDock(const common::XY<double>& location)
	{
		int currentTurn = (int)game::avatar::Statistics::GetCurrent(game::avatar::Statistic::TURNS_REMAINING);
		game::Islands::AddVisit(
			location,
			currentTurn);
		game::islands::Quests::Update(location);
		data::game::avatar::Dock::SetLocation(location);
		return true;
	}

	bool Dock()
	{
		if (GetDockedLocation().has_value())
		{
			return false;//TODO: should this be true because we are docked, or false because we cannot dock?
		}
		auto dockables = game::Islands::GetDockableIslands();
		if (!dockables.empty())
		{
			return DoDock(dockables.front().absoluteLocation);
		}
		return false;
	}

	std::optional<common::XY<double>> GetDockedLocation()
	{
		return data::game::avatar::Dock::GetLocation();
	}

	bool Undock()
	{
		if (GetDockedLocation().has_value())
		{
			//TODO: statistic for undocking
			data::game::avatar::Dock::SetLocation(std::nullopt);
			return true;
		}
		return false;
	}

	std::optional<common::XY<double>> GetDestination()
	{
		return data::game::avatar::Destination::Read();
	}

	void SetDestination(const std::optional<common::XY<double>>& destination)
	{
		if (destination)
		{
			data::game::avatar::Destination::Write(destination.value());
			return;
		}
		data::game::avatar::Destination::Clear();
	}

}