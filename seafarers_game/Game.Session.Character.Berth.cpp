#include <Data.Game.Character.Ship.h>
#include "Game.Session.Character.Berth.h"
namespace game::session::character
{
	using ShipData = data::game::character::Ship;

	static std::optional<int> TryGetShipId(int characterId)
	{
		
		return ShipData::ReadShipForCharacter(characterId);
	}

	int Berth::GetShipId() const
	{
		return TryGetShipId(characterId).value();
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