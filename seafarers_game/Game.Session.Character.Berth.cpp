#include "Game.Characters.Ships.h"
#include "Game.Session.Character.Berth.h"
namespace game::session::character
{
	Berth::Berth(int characterId)
		: characterId(characterId)
	{

	}

	std::optional<game::session::Ship> Berth::GetShip() const
	{
		auto shipId = game::characters::Ships::ReadShipId(characterId);
		if (shipId)
		{
			return game::session::Ship(shipId.value());
		}
		return std::nullopt;
	}


	std::optional<BerthType> Berth::GetBerthType() const
	{
		return game::characters::Ships::ReadBerthType(characterId);
	}
}