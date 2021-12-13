#include "Game.Characters.Ships.h"
#include "Game.Session.Ship.Berth.h"
namespace game::session::ship
{
	Berth::Berth(int characterId)
		: characterId(characterId)
	{

	}

	int Berth::GetCharacterId() const
	{
		return characterId;
	}

	std::optional<BerthType> Berth::GetBerthType() const
	{
		return game::characters::Ships::ReadBerthType(characterId);
	}

}