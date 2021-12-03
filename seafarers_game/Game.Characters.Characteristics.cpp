#include <Data.Game.Character.Characteristics.h>
#include "Game.Characters.Characteristics.h"
namespace game::characters
{
	void Characteristics::Write(int characterId, const Characteristic& characteristic, int value)
	{
		data::game::character::Characteristics::Write(characterId, (int)characteristic, value);
	}

	std::optional<int> Characteristics::Read(int characterId, const Characteristic& characteristic)
	{
		return data::game::character::Characteristics::Read(characterId, (int)characteristic);
	}

	std::map<Characteristic, int> Characteristics::Read(int characterId)
	{
		std::map<Characteristic, int> result;
		auto characteristics = data::game::character::Characteristics::Read(characterId);
		for (auto characteristic : characteristics)
		{
			result[(Characteristic)characteristic.first] = characteristic.second;
		}
		return result;
	}
}