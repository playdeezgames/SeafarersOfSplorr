#include <Common.Utility.Optional.h>
#include <Data.Game.Character.Ship.h>
#include "Game.Characters.Ships.h"
#include "Game.Characters.Items.h"
#include "Game.Ship.h"
#include "Game.ShipTypes.h"
namespace game::character
{
	void Ship::Write(int characterId, int shipId, const BerthType& berthType)
	{
		data::game::character::Ship::Write(characterId, 
			{ 
				shipId,
				(int)berthType
			});
	}

	static int ToShipId(const data::game::character::Ship& ship)
	{
		return ship.shipId;
	}

	static BerthType ToBerthType(const data::game::character::Ship& ship)
	{
		return (BerthType)ship.berthType;
	}

	std::optional<int> Ship::ReadShipId(int characterId)
	{
		return common::utility::Optional::Map<data::game::character::Ship, int>(
			data::game::character::Ship::ReadForCharacter(characterId),
			ToShipId);
	}


	static double GetAvailableTonnage(int characterId, int shipId)
	{
		return common::utility::Optional::Map<ShipType, double>(
			game::Ship::GetShipType(shipId),
			[characterId](const ShipType& shipType)
			{
				return
					game::ShipTypes::GetTotalTonnage(shipType) -
					game::character::Items::TotalTonnage(characterId);
			}).value_or(0.0);
	}

	std::optional<double> Ship::AvailableTonnage(int characterId)
	{
		return common::utility::Optional::Map<int, double>(
			ReadShipId(characterId),
			[characterId](int shipId) { return GetAvailableTonnage(characterId, shipId); });
	}

	std::optional<BerthType> Ship::ReadBerthType(int characterId)
	{
		return common::utility::Optional::Map<data::game::character::Ship, BerthType>(
			data::game::character::Ship::ReadForCharacter(characterId),
			ToBerthType);
	}

}