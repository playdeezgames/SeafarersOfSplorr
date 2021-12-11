#pragma once
#include "Game.Session.Character.Characteristic.h"
#include <list>
namespace game::session
{
	struct Character
	{
		Character(int);
		std::list<game::session::character::Characteristic> GetCharacteristics() const;
	private:
		int characterId;
	};
}
