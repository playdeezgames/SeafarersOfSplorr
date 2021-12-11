#pragma once
#include "Game.Session.Character.Characteristic.h"
#include "Game.Session.Character.HitPoints.h"
#include "Game.Session.Ship.h"
#include <list>
#include <optional>
namespace game::session
{
	struct Character
	{
		Character(int);
		std::list<character::Characteristic> GetCharacteristics() const;
		character::HitPoints GetHitpoints() const;
		std::optional<Ship> GetShip() const;
	private:
		int characterId;
	};
}
