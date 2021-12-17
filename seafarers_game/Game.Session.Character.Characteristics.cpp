#include <Data.Game.Character.Characteristic.h>
#include "Game.Session.Character.Characteristics.h"
namespace game::session::character
{
	Characteristics::Characteristics(int characterId)
		: characterId(characterId)
	{

	}

	std::list<Characteristic> Characteristics::GetAll() const
	{
		auto characteristics = data::game::character::Characteristic::Read(characterId);
		std::list<Characteristic> result;
		for (auto characteristic : characteristics)
		{
			result.push_back(Characteristic(characterId, (game::Characteristic)characteristic.first));
		}
		return result;
	}

	Characteristic Characteristics::GetCharacteristic(const game::Characteristic& characteristic) const
	{
		return Characteristic(characterId, characteristic);
	}
}