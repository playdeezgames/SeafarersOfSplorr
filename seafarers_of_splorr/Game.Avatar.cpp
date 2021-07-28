#include "Application.UIState.h"
#include "Common.Data.h"
#include "Common.RNG.h"
#include "Common.Utility.h"
#include "Data.Game.Avatar.h"
#include "Data.Game.Avatar.Dock.h"
#include "Game.Avatar.h"
#include "Game.Avatar.Items.h"
#include "Game.Avatar.Quest.h"
#include "Game.Avatar.Ship.h"
#include "Game.Avatar.Statistics.h"
#include "Game.Heading.h"
#include "Game.Islands.h"
#include "Game.Islands.Quests.h"
#include "Game.Ships.h"
#include "Game.World.h"
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
		const double EAT_BENEFIT = 10.0;
		game::avatar::Statistics::ChangeCurrent(game::avatar::Statistic::TURNS_REMAINING, TURN_DELTA);
		if (game::avatar::Statistics::GetCurrent(game::avatar::Statistic::HUNGER) > game::avatar::Statistics::GetMinimum(game::avatar::Statistic::HUNGER))
		{
			game::avatar::Statistics::ChangeCurrent(game::avatar::Statistic::HUNGER, HUNGER_DELTA);
		}
		else
		{
			game::avatar::Statistics::ChangeCurrent(game::avatar::Statistic::HEALTH, HEALTH_DELTA);
		}
		if (game::avatar::Statistics::NeedToEat(EAT_BENEFIT))
		{
			auto rations = game::avatar::Items::Read(game::Item::RATIONS);
			if (rations > 0)
			{
				game::avatar::Statistics::Eat(EAT_BENEFIT);
				game::avatar::Items::Remove(game::Item::RATIONS, 1);
			}
		}
	}

	void Move()
	{
		auto avatar = data::game::Avatar::Read().value();
		auto shipDescriptor = game::Ships::Read(game::avatar::Ship::Read());
		common::XY<double> delta = 
			game::Heading::DegreesToXY(avatar.heading) * 
			avatar.speed * 
			shipDescriptor.properties.find(game::ship::Property::SPEED_FACTOR)->second;
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
		game::avatar::Quest::CompleteQuest(location);
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

}