#include <Data.Game.Avatar.ShipStatistic.h>
#include "Game.ShipStatistic.h"
#include "Game.Avatar.ShipStatistics.h"
namespace game::avatar
{
	static double GetMaximumFouling()
	{
		double portFouling = data::game::avatar::ShipStatistic::Read((int)ShipStatistic::PORT_FOULING).value().maximum.value();
		double starboardFouling = data::game::avatar::ShipStatistic::Read((int)ShipStatistic::STARBOARD_FOULING).value().maximum.value();
		return portFouling + starboardFouling;
	}

	double ShipStatistics::GetFouling()
	{
		double portFouling = data::game::avatar::ShipStatistic::Read((int)ShipStatistic::PORT_FOULING).value().current;
		double starboardFouling = data::game::avatar::ShipStatistic::Read((int)ShipStatistic::STARBOARD_FOULING).value().current;
		return portFouling + starboardFouling;
	}

	double ShipStatistics::GetFoulingPercentage()
	{
		return 100.0 * GetFouling() / GetMaximumFouling();
	}


	void ShipStatistics::IncreaseFouling(double multiplier)
	{
		auto portFouling = data::game::avatar::ShipStatistic::Read((int)ShipStatistic::PORT_FOULING).value();
		auto starboardFouling = data::game::avatar::ShipStatistic::Read((int)ShipStatistic::STARBOARD_FOULING).value();
		auto foulingRate = data::game::avatar::ShipStatistic::Read((int)ShipStatistic::FOULING_RATE).value().current;

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
		data::game::avatar::ShipStatistic::Write((int)ShipStatistic::PORT_FOULING, portFouling);
		data::game::avatar::ShipStatistic::Write((int)ShipStatistic::STARBOARD_FOULING, starboardFouling);
	}
}