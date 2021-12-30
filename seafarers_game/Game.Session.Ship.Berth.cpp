#include <Common.Utility.Optional.h>
#include <Data.Game.Character.Ship.h>
#include "Game.Session.Ship.Berth.h"
namespace game::session::ship
{
	using OptUtil = common::utility::Optional;
	using CharacterShipData = data::game::character::Ship;

	static std::optional<BerthType> TryGetBerthType(int characterId)
	{
		return OptUtil::Map<int, BerthType>(
			CharacterShipData::ReadBerthForCharacter(characterId),
			[](int value) { return (BerthType)value; });
	}

	BerthType Berth::GetBerthType() const
	{
		return TryGetBerthType(characterId).value();
	}
}