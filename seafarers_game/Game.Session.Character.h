#pragma once
#include "Game.Session.Character.Characteristic.h"
#include "Game.Session.Character.HitPoints.h"
#include "Game.Session.Character.Berth.h"
#include <list>
#include <optional>
namespace game::session
{
	struct Character
	{
		Character(int);
		std::list<character::Characteristic> GetCharacteristics() const;
		character::HitPoints GetHitpoints() const;
		std::optional<character::Berth> GetBerth() const;
	private:
		int characterId;
	};
}
