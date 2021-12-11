#include "Game.Characters.Ships.h"
#include "Game.Session.Character.Berth.h"
namespace game::session::character
{
	Berth::Berth(int characterId)
		: characterId(characterId)
	{

	}

	std::optional<int> Berth::GetShipId() const
	{
		return game::characters::Ships::ReadShipId(characterId);
	}

	std::optional<BerthType> Berth::GetBerthType() const
	{
		return game::characters::Ships::ReadBerthType(characterId);
	}
}