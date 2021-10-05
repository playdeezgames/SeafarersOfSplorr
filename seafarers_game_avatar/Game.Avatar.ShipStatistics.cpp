#include <Common.Data.h>
#include <Data.Game.Ship.Statistic.h>
#include "Game.Avatar.Log.h"
#include "Game.Avatar.ShipStatistics.h"
#include "Game.Avatar.Ship.h"
#include "Game.Colors.h"
#include "Game.ShipStatistic.h"
#include <map>
namespace game::avatar
{
	static double GetMaximumFouling()
	{
		int shipId = game::avatar::Ship::Read().value();
		double portFouling = data::game::ship::Statistic::Read(shipId, (int)ShipStatistic::PORT_FOULING).value().maximum.value();
		double starboardFouling = data::game::ship::Statistic::Read(shipId, (int)ShipStatistic::STARBOARD_FOULING).value().maximum.value();
		return portFouling + starboardFouling;
	}

	double ShipStatistics::GetFouling()
	{
		int shipId = game::avatar::Ship::Read().value();
		double portFouling = data::game::ship::Statistic::Read(shipId, (int)ShipStatistic::PORT_FOULING).value().current;
		double starboardFouling = data::game::ship::Statistic::Read(shipId, (int)ShipStatistic::STARBOARD_FOULING).value().current;
		return portFouling + starboardFouling;
	}

	double ShipStatistics::GetFoulingPercentage(const game::Side& side)
	{
		int shipId = game::avatar::Ship::Read().value();
		double maximum = data::game::ship::Statistic::Read(shipId, (int)side).value().maximum.value();
		double current = data::game::ship::Statistic::Read(shipId, (int)side).value().current;
		return common::Data::ToPercentage(current, maximum).value();
	}


	double ShipStatistics::GetFoulingPercentage()
	{
		return common::Data::ToPercentage(GetFouling(), GetMaximumFouling()).value();
	}


	void ShipStatistics::IncreaseFouling(double multiplier)
	{
		int shipId = game::avatar::Ship::Read().value();
		auto portFouling = data::game::ship::Statistic::Read(shipId, (int)ShipStatistic::PORT_FOULING).value();
		auto starboardFouling = data::game::ship::Statistic::Read(shipId, (int)ShipStatistic::STARBOARD_FOULING).value();
		auto foulingRate = data::game::ship::Statistic::Read(shipId, (int)ShipStatistic::FOULING_RATE).value().current;

		portFouling.current += (foulingRate * multiplier);
		if (portFouling.maximum.has_value() && portFouling.current > portFouling.maximum.value())
		{
			portFouling.current = portFouling.maximum.value();
		}
		starboardFouling.current += (foulingRate * multiplier);
		if (starboardFouling.maximum.has_value() && starboardFouling.current > starboardFouling.maximum.value())
		{
			starboardFouling.current = starboardFouling.maximum.value();
		}

		data::game::ship::Statistic::Write(shipId, (int)ShipStatistic::PORT_FOULING, portFouling);
		data::game::ship::Statistic::Write(shipId, (int)ShipStatistic::STARBOARD_FOULING, starboardFouling);
	}

	struct SideDescriptor
	{
		ShipStatistic statistic;
		std::string cleaningMessage;
	};

	static const std::map<game::Side, SideDescriptor> foulingTable =
	{
		{ Side::PORT, {ShipStatistic::PORT_FOULING, "You clean the port hull."}},
		{ Side::STARBOARD, {ShipStatistic::STARBOARD_FOULING, "You clean the starboard hull."}},
	};

	void ShipStatistics::CleanHull(const Side& side)
	{
		int shipId = game::avatar::Ship::Read().value();
		auto descriptor = foulingTable.find(side)->second;
		auto fouling = data::game::ship::Statistic::Read(shipId, (int)descriptor.statistic).value();
		fouling.current = fouling.minimum.value_or(0.0);
		data::game::ship::Statistic::Write(shipId, (int)descriptor.statistic, fouling);
		Log::Write({Colors::GRAY, descriptor.cleaningMessage});
	}
}