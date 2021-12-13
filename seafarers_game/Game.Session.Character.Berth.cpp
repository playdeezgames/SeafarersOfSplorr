#include "Game.Characters.Ships.h"
#include "Game.Session.Character.Berth.h"
namespace game::session::character
{
	Berth::Berth(int characterId)
		: characterId(characterId)
	{

	}

	static std::optional<game::session::Ship> TryGetShip(int characterId)
	{
		auto shipId = game::characters::Ships::ReadShipId(characterId);
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
		return game::characters::Ships::ReadBerthType(characterId);
	}

	BerthType Berth::GetBerthType() const
	{
		return TryGetBerthType(characterId).value();
	}
}