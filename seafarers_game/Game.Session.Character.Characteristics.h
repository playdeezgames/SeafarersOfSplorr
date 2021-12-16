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
	private:
		int characterId;
	};
}
