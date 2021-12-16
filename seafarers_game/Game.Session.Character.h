#pragma once
#include "Game.Characters.Action.h"
#include "Game.Session.Character.Berth.h"
#include "Game.Session.Character.Characteristic.h"
#include "Game.Session.Character.Characteristics.h"
#include "Game.Session.Character.HitPoints.h"
#include "Game.Session.Character.Items.h"
#include "Game.Session.FishGame.h"
#include "Game.Session.Islands.h"
#include "Game.Session.Character.LegacyItems.h"
#include "Game.Session.Character.Messages.h"
#include "Game.Session.Character.Quest.h"
#include <list>
#include <optional>
namespace game::session
{
	struct Character
	{
		Character(int);
		std::list<character::Characteristic> GetCharacteristicsLegacy() const;
		character::Characteristics GetCharacteristics() const;
		character::HitPoints GetHitpoints() const;
		character::Berth GetBerth() const;
		character::LegacyItems GetLegacyItems() const;
		std::optional<character::Quest> TryGetQuest() const;
		character::Quest GetQuest() const;
		void DoAction(const game::characters::Action&) const;
		character::Messages GetMessages() const;
		Islands GetKnownIslands() const;
		FishGame GetFishGame() const;
		double GetAvailableTonnage() const;
		double GetMoney() const;
		bool IsDead() const;
		character::Items GetItems() const;
	private:
		int characterId;
	};
}
