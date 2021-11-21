#include <Common.Utility.h>
#include <Common.Utility.Optional.h>
#include <Data.Game.Avatar.Ship.h>
#include <Data.Game.Ship.Statistic.h>
#include <Data.Game.Ship.h>
#include "Game.Avatar.Ship.h"
#include "Game.Avatar.Items.h"
#include <Game.Ship.h>
#include <Game.ShipTypes.h>
namespace game::avatar
{
	void Ship::Write(int avatarId, const Ship& ship)
	{
		data::game::avatar::Ship::Write(avatarId, 
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

	std::optional<Ship> Ship::Read(int avatarId)
	{
		return common::utility::Optional::Map<data::game::avatar::Ship, Ship>(
			data::game::avatar::Ship::Read(avatarId),
			ToShip
			);
	}

	std::optional<int> Ship::ReadShipId(int avatarId)
	{
		return common::utility::Optional::Map<Ship, int>(
			Read(avatarId), 
			[](const Ship& ship) { return ship.shipId;  });
	}


	static double GetAvailableTonnage(int avatarId, const Ship& ship)
	{
		return common::utility::Optional::Map<ShipType, double>(
			game::Ship::GetShipType(ship.shipId),
			[avatarId](const ShipType& shipType)
			{
				return
					game::ShipTypes::GetTotalTonnage(shipType) -
					game::avatar::Items::TotalTonnage(avatarId);
			}).value_or(0.0);
	}

	std::optional<double> Ship::AvailableTonnage(int avatarId)
	{
		return common::utility::Optional::Map<Ship, double>(
			Read(avatarId),
			[avatarId](const Ship& ship) { return GetAvailableTonnage(avatarId, ship); });
	}
}