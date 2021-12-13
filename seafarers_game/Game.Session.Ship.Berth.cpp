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

	static std::optional<BerthType> TryGetBerthType(int characterId)
	{
		return game::characters::Ships::ReadBerthType(characterId);
	}

	BerthType Berth::GetBerthType() const
	{
		return TryGetBerthType(characterId).value();
	}
}