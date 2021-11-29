#include <Common.Data.h>
#include <Data.Game.Ship.Statistic.h>
#include "Game.Character.ShipStatistics.h"
#include "Game.Character.Ship.h"
#include "Game.Colors.h"
#include "Game.ShipStatistic.h"
#include <map>
namespace game::character
{
	static data::game::ship::Statistic GetStatistic(int shipId, const ShipStatistic& statistic)
	{
		return data::game::ship::Statistic::Read(shipId, (int)statistic).value();
	}

	static double GetMaximumFouling(int shipId)
	{
		double portFouling = GetStatistic(shipId, ShipStatistic::PORT_FOULING).maximum.value();
		double starboardFouling = GetStatistic(shipId, ShipStatistic::STARBOARD_FOULING).maximum.value();
		return portFouling + starboardFouling;
	}

	double ShipStatistics::GetFouling(int shipId)
	{
		double portFouling = GetStatistic(shipId, ShipStatistic::PORT_FOULING).current;
		double starboardFouling = GetStatistic(shipId, ShipStatistic::STARBOARD_FOULING).current;
		return portFouling + starboardFouling;
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

	double ShipStatistics::GetFoulingPercentage(int shipId, const game::Side& side)
	{
		auto statistic = GetStatistic(shipId, foulingTable.find(side)->second.statistic);
		return common::Data::ToPercentage(statistic.current, statistic.maximum.value()).value();
	}

	double ShipStatistics::GetFoulingPercentage(int shipId)
	{
		return common::Data::ToPercentage(GetFouling(shipId), GetMaximumFouling(shipId)).value();
	}

	static void IncreaseFouling(int shipId, const ShipStatistic& statistic, double amount)
	{
		auto fouling = GetStatistic(shipId, statistic);
		fouling.current += amount;
		if (fouling.maximum.has_value() && fouling.current > fouling.maximum.value())
		{
			fouling.current = fouling.maximum.value();
		}
		data::game::ship::Statistic::Write(shipId, (int)statistic, fouling);
	}

	void ShipStatistics::IncreaseFouling(int shipId, double multiplier)
	{
		auto foulingRate = GetStatistic(shipId, ShipStatistic::FOULING_RATE).current * multiplier;
		game::character::IncreaseFouling(shipId, ShipStatistic::PORT_FOULING, foulingRate);
		game::character::IncreaseFouling(shipId, ShipStatistic::STARBOARD_FOULING, foulingRate);
	}

	void ShipStatistics::CleanHull(int shipId, const Side& side)
	{
		auto descriptor = foulingTable.find(side)->second;
		auto fouling = GetStatistic(shipId, descriptor.statistic);
		fouling.current = fouling.minimum.value_or(0.0);
		data::game::ship::Statistic::Write(shipId, (int)descriptor.statistic, fouling);
	}
}