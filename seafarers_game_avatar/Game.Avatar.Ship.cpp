#include <Data.Game.Avatar.Ship.h>
#include <Data.Game.Avatar.ShipStatistic.h>
#include "Game.Avatar.Ship.h"
#include "Game.Avatar.Items.h"
#include "Game.Ships.h"
namespace game::avatar
{
	void Ship::Reset(const game::Difficulty&)
	{
		auto ship = game::Ships::GenerateForAvatar();
		Write(ship);
	}

	void Ship::Write(const game::Ship& ship)
	{
		data::game::avatar::Ship::Write((int)ship);
		auto statistics = Ships::GetStatistics(ship);
		for (auto statistic : statistics)
		{
			data::game::avatar::ShipStatistic::Write((int)statistic.first, 
				{
					statistic.second.minimum,
					statistic.second.maximum,
					statistic.second.initial
				});
		}
	}

	game::Ship Ship::Read()
	{
		return (game::Ship)data::game::avatar::Ship::Read();
	}

	double Ship::AvailableTonnage()
	{
		return 
			game::Ships::GetTotalTonnage(Read()) -
			game::avatar::Items::TotalTonnage();
	}
}