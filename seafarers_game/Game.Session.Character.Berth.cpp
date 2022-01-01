#include <Data.Game.Character.Ship.h>
#include "Game.Session.Character.Berth.h"
namespace game::session::character
{
	using ShipData = data::game::character::Ship;

	static std::optional<game::session::Ship> TryGetShip(int characterId)
	{
		
		auto shipId = ShipData::ReadShipForCharacter(characterId);
		if (shipId)
		{
			return Ship(shipId.value());
		}
		return std::nullopt;
	}

	session::Ship Berth::GetShip() const
	{
		return TryGetShip(characterId).value();
	}

	static std::optional<BerthType> TryGetBerthType(int characterId)
	{
		auto berthType = ShipData::ReadBerthForCharacter(characterId);
		if (berthType)
		{
			return (BerthType)*berthType;
		}
		return std::nullopt;
	}

	BerthType Berth::GetBerthType() const
	{
		return TryGetBerthType(characterId).value();
	}
}