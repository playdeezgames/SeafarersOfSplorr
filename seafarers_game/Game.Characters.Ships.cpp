#include <Common.Utility.Optional.h>
#include <Data.Game.Character.Ship.h>
#include "Game.Characters.Ships.h"
#include "Game.Characters.Items.h"
#include "Game.Ship.h"
#include "Game.ShipTypes.h"
namespace game::characters
{
	void Ships::Write(int characterId, int shipId, const BerthType& berthType)
	{
		data::game::character::Ship::Write(characterId, 
			 
				shipId,
				(int)berthType
			);
	}

	std::optional<int> Ships::ReadShipId(int characterId)
	{
		return
			data::game::character::Ship::ReadShipForCharacter(characterId);
	}


	static double GetAvailableTonnage(int characterId, int shipId)
	{
		return common::utility::Optional::Map<ShipType, double>(
			game::Ship::GetShipType(shipId),
			[characterId](const ShipType& shipType)
			{
				return
					game::ShipTypes::GetTotalTonnage(shipType) -
					game::characters::Items::TotalTonnage(characterId);
			}).value_or(0.0);
	}

	std::optional<double> Ships::AvailableTonnage(int characterId)
	{
		return common::utility::Optional::Map<int, double>(
			ReadShipId(characterId),
			[characterId](int shipId) { return GetAvailableTonnage(characterId, shipId); });
	}

	std::optional<BerthType> Ships::ReadBerthType(int characterId)
	{
		auto berth = data::game::character::Ship::ReadBerthForCharacter(characterId);
		if (berth)
		{
			return (BerthType)berth.value();
		}
		return std::nullopt;
	}

}