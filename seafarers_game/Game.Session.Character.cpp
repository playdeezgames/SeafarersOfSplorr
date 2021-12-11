#include "Game.Characters.Characteristics.h"
#include "Game.Session.Character.h"
namespace game::session
{
	Character::Character(int characterId)
		: characterId(characterId)
	{

	}

	std::list<game::session::character::Characteristic> Character::GetCharacteristics() const
	{
		std::list<game::session::character::Characteristic> result;
		auto characteristics = game::characters::Characteristics::Read(characterId);
		for (auto characteristic : characteristics)
		{
			result.push_back(character::Characteristic(characterId, characteristic.first));
		}
		return result;
	}

	character::HitPoints Character::GetHitpoints() const
	{
		return character::HitPoints(characterId);
	}
}