#pragma once
#include "Game.Characteristic.h"
#include "Game.Session.Character.Characteristic.h"
#include <list>
namespace game::session::character
{
	struct Characteristics
	{
		Characteristics(int);
		std::list<Characteristic> GetAll() const;
		Characteristic GetCharacteristic(const game::Characteristic&) const;
	private:
		int characterId;
	};
}
