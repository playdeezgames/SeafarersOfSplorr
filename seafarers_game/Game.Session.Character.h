#pragma once
#include "Game.Characters.Action.h"
#include "Game.Characters.State.h"
#include "Game.Session.Character.Berth.h"
#include "Game.Session.Character.Characteristic.h"
#include "Game.Session.Character.Characteristics.h"
#include "Game.Session.Character.Counters.h"
#include "Game.Session.Character.Flags.h"
#include "Game.Session.Character.HitPoints.h"
#include "Game.Session.Character.KnownIslands.h"
#include "Game.Session.Character.Items.h"
#include "Game.Session.Character.LegacyItems.h"
#include "Game.Session.Character.Messages.h"
#include "Game.Session.Character.Quest.h"
#include "Game.Session.FishGame.h"
#include <list>
#include <optional>
namespace game::session
{
	struct Player;
	struct Character
	{
		constexpr Character(int characterId) : characterId(characterId) {}
		constexpr explicit operator int() const { return characterId; }
		constexpr character::KnownIslands GetKnownIslands() const { return character::KnownIslands(characterId); }

		character::Characteristics GetCharacteristics() const;
		character::HitPoints GetHitpoints() const;
		character::Berth GetBerth() const;
		std::optional<character::Quest> TryGetQuest() const;
		character::Quest GetQuest() const;
		void DoAction(const game::characters::Action&) const;
		character::Messages GetMessages() const;
		FishGame GetFishGame() const;
		double GetAvailableTonnage() const;
		bool IsDead() const;
		character::Items GetItems() const;
		character::Counters GetCounters() const;
		character::Flags GetFlags() const;
		std::string GetName() const;
		std::optional<characters::State> TryGetState() const;
		characters::State GetState() const;


		void SetIsland(Island island) const;
		void ClearIsland() const;
		std::optional<Island> TryGetIsland() const;
		Island GetIsland() const;
		character::LegacyItems GetLegacyItems() const;
	private:
		int characterId;
	};
}
