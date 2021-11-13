#include <Common.Utility.h>
#include <Common.Utility.Optional.h>
#include <Data.Game.Avatar.Ship.h>
#include <Data.Game.Ship.Statistic.h>
#include <Data.Game.Player.h>
#include <Data.Game.Ship.h>
#include "Game.Avatar.Ship.h"
#include "Game.Avatar.Items.h"
#include <Game.Player.h>
#include <Game.Ship.h>
#include <Game.ShipTypes.h>
namespace game::avatar//20211018
{
	void Ship::Reset(const game::Difficulty&)
	{
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
		return common::utility::Optional::Map<data::game::avatar::Ship, Ship>(
			data::game::avatar::Ship::Read(data::game::Player::GetAvatarId()),
			ToShip
			);
	}

	std::optional<int> Ship::ReadShipId()
	{
		return common::utility::Optional::Map<Ship, int>(
			Read(), 
			[](const Ship& ship) { return ship.shipId;  });
	}


	static double GetAvailableTonnage(const Ship& ship)
	{
		return common::utility::Optional::Map<ShipType, double>(
			game::Ship::GetShipType(ship.shipId),
			[](const ShipType& shipType)
			{
				return
					game::ShipTypes::GetTotalTonnage(shipType) -
					game::avatar::Items::TotalTonnage(game::Player::GetAvatarId());
			}).value_or(0.0);
	}

	std::optional<double> Ship::AvailableTonnage()
	{
		return common::utility::Optional::Map<Ship, double>(
			Read(),
			GetAvailableTonnage);
	}
}