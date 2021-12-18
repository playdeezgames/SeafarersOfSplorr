#pragma once
#include "Game.Characters.Action.h"
#include "Game.Session.Character.Berth.h"
#include "Game.Session.Character.Characteristic.h"
#include "Game.Session.Character.Characteristics.h"
#include "Game.Session.Character.Counters.h"
#include "Game.Session.Character.Flags.h"
#include "Game.Session.Character.HitPoints.h"
#include "Game.Session.Character.Items.h"
#include "Game.Session.Character.LegacyItems.h"
#include "Game.Session.Character.Messages.h"
#include "Game.Session.Character.Quest.h"
#include "Game.Session.FishGame.h"
#include "Game.Session.Islands.h"
#include <list>
#include <optional>
namespace game::session
{
	struct Player;
	struct Character
	{
		Character(int);
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
		void SetIsland(Island island) const;
		character::Counters GetCounters() const;
		character::Flags GetFlags() const;
		std::string GetName() const;
	private:
		friend struct game::session::Player;
		int characterId;
	};
}
