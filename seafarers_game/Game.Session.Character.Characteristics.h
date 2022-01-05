#pragma once
#include "Game.Characteristic.h"
#include "Game.Session.Character.Characteristic.h"
#include <vector>
namespace game::session::character
{
	struct Characteristics
	{
		constexpr explicit Characteristics(int characterId) : characterId(characterId) { }
		std::vector<Characteristic> GetAll() const;
		constexpr Characteristic GetCharacteristic(const game::Characteristic& characteristic) const
		{
			return Characteristic(characterId, characteristic);
		}
	private:
		int characterId;
	};
}
