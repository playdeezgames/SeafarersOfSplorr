#include <Data.Game.Character.Ship.h>
#include "Game.Session.Character.Berth.h"
namespace game::session::character
{
	Berth::Berth(int characterId)
		: characterId(characterId)
	{

	}

	static std::optional<game::session::Ship> TryGetShip(int characterId)
	{
		
		auto shipId = data::game::character::Ship::ReadShipForCharacter(characterId);
		if (shipId)
		{
			return game::session::Ship(shipId.value());
		}
		return std::nullopt;
	}

	session::Ship Berth::GetShip() const
	{
		return TryGetShip(characterId).value();
	}

	static std::optional<BerthType> TryGetBerthType(int characterId)
	{
		auto berthType = data::game::character::Ship::ReadBerthForCharacter(characterId);
		if (berthType)
		{
			return (BerthType)berthType.value();
		}
		return std::nullopt;
	}

	BerthType Berth::GetBerthType() const
	{
		return TryGetBerthType(characterId).value();
	}
}