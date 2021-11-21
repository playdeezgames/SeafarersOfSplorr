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
	void Ship::Write(int avatarId, int shipId, const BerthType& berthType)
	{
		data::game::avatar::Ship::Write(avatarId, 
			{ 
				shipId,
				(int)berthType
			});
	}

	static int ToShipId(const data::game::avatar::Ship& ship)
	{
		return ship.shipId;
	}

	static BerthType ToBerthType(const data::game::avatar::Ship& ship)
	{
		return (BerthType)ship.berthType;
	}

	std::optional<int> Ship::ReadShipId(int avatarId)
	{
		return common::utility::Optional::Map<data::game::avatar::Ship, int>(
			data::game::avatar::Ship::Read(avatarId),
			ToShipId);
	}


	static double GetAvailableTonnage(int avatarId, int shipId)
	{
		return common::utility::Optional::Map<ShipType, double>(
			game::Ship::GetShipType(shipId),
			[avatarId](const ShipType& shipType)
			{
				return
					game::ShipTypes::GetTotalTonnage(shipType) -
					game::avatar::Items::TotalTonnage(avatarId);
			}).value_or(0.0);
	}

	std::optional<double> Ship::AvailableTonnage(int avatarId)
	{
		return common::utility::Optional::Map<int, double>(
			ReadShipId(avatarId),
			[avatarId](int shipId) { return GetAvailableTonnage(avatarId, shipId); });
	}

	std::optional<BerthType> Ship::ReadBerthType(int avatarId)
	{
		return common::utility::Optional::Map<data::game::avatar::Ship, BerthType>(
			data::game::avatar::Ship::Read(avatarId),
			ToBerthType);
	}

}