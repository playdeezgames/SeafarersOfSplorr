#include <Data.Game.Avatar.Ship.h>
#include <Data.Game.Avatar.ShipStatistic.h>
#include <Data.Game.Player.h>
#include <Data.Game.Ship.h>
#include "Game.Avatar.Ship.h"
#include "Game.Avatar.Items.h"
#include "Game.ShipTypes.h"
namespace game::avatar
{
	void Ship::Reset(const game::Difficulty&)
	{
		auto ship = game::ShipTypes::GenerateForAvatar();
		Write(ship);
	}

	void Ship::Write(const game::ShipType& ship)
	{
		auto shipId = data::game::Ship::NextId();//TODO: this is STILL wrong
		data::game::avatar::Ship::Write(data::game::Player::GetAvatarId(), (int)ship, shipId);
		auto statistics = ShipTypes::GetStatistics(ship);
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

	game::ShipType Ship::Read()
	{
		return (game::ShipType)data::game::avatar::Ship::ReadShipType(data::game::Player::GetAvatarId());
	}

	double Ship::AvailableTonnage()
	{
		return 
			game::ShipTypes::GetTotalTonnage(Read()) -
			game::avatar::Items::TotalTonnage();
	}
}