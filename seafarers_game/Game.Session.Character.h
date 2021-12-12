#pragma once
#include "Game.Characters.Action.h"
#include "Game.Session.Character.Berth.h"
#include "Game.Session.Character.Characteristic.h"
#include "Game.Session.Character.HitPoints.h"
#include "Game.Session.Islands.h"
#include "Game.Session.Character.Items.h"
#include "Game.Session.Character.Messages.h"
#include "Game.Session.Character.Quest.h"
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
		character::Items GetItems() const;
		std::optional<character::Quest> GetQuest() const;
		void DoAction(const game::characters::Action&) const;
		character::Messages GetMessages() const;
		Islands GetKnownIslands() const;
	private:
		int characterId;
	};
}
