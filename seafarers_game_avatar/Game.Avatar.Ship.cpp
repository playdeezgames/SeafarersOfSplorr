#include <Common.Utility.h>
#include <Data.Game.Avatar.Ship.h>
#include <Data.Game.Ship.Statistic.h>
#include <Data.Game.Player.h>
#include <Data.Game.Ship.h>
#include "Game.Avatar.Ship.h"
#include "Game.Avatar.Items.h"
#include <Game.Ship.h>
#include <Game.ShipTypes.h>
namespace game::avatar//20211018
{
	void Ship::Reset(const game::Difficulty&)
	{
		data::game::Ship::Clear();
	}

	void Ship::Write(const Ship& ship)
	{
		data::game::avatar::Ship::Write(data::game::Player::GetAvatarId(), 
			{ 
				ship.shipId,
				(int)ship.berthType
			});
	}

	static Ship ToShip(const data::game::avatar::Ship& ship)
	{
		return 
		{
			ship.shipId,
			(BerthType)ship.berthType
		};
	}

	std::optional<Ship> Ship::Read()
	{
		return common::Utility::MapOptional<data::game::avatar::Ship, Ship>(
			data::game::avatar::Ship::Read(data::game::Player::GetAvatarId()),
			ToShip
			);
	}

	std::optional<int> Ship::ReadShipId()
	{
		return common::Utility::MapOptional<Ship, int>(
			Read(), 
			[](const Ship& ship) { return ship.shipId;  });
	}


	static double GetAvailableTonnage(const Ship& ship)
	{
		return common::Utility::MapOptional<ShipType, double>(
			game::Ship::GetShipType(ship.shipId),
			[](const ShipType& shipType)
			{
				return
					game::ShipTypes::GetTotalTonnage(shipType) -
					game::avatar::Items::TotalTonnage();
			}).value_or(0.0);
	}

	std::optional<double> Ship::AvailableTonnage()
	{
		return common::Utility::MapOptional<Ship, double>(
			Read(),
			GetAvailableTonnage);
	}
}