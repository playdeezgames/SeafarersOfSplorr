#pragma once
#include "Game.Session.Character.Berth.h"
#include "Game.Session.Character.Characteristic.h"
#include "Game.Session.Character.Characteristics.h"
#include "Game.Session.Character.Counters.h"
#include "Game.Session.Character.Deliveries.h"
#include "Game.Session.Character.Flags.h"
#include "Game.Session.Character.HitPoints.h"
#include "Game.Session.Character.KnownIslands.h"
#include "Game.Session.Character.Items.h"
#include "Game.Session.Character.Marks.h"
#include "Game.Session.Character.Messages.h"
#include "Game.Session.Character.Plights.h"
#include "Game.Session.Character.Skills.h"
#include "Game.Session.Island.h"
#include "Game.Session.Tribe.h"
#include <optional>
namespace game::session
{
	struct Character
	{
		constexpr explicit Character(int characterId) : characterId(characterId) {}
		constexpr explicit operator int() const { return characterId; }
		constexpr character::KnownIslands GetKnownIslands() const { return character::KnownIslands(characterId); }
		constexpr character::Plights GetPlights() const { return character::Plights(characterId); }
		constexpr static Character ToCharacter(int characterId) { return Character(characterId); }
		constexpr character::Characteristics GetCharacteristics() const { return character::Characteristics(characterId); }
		constexpr character::HitPoints GetHitpoints() const { return character::HitPoints(characterId); }
		constexpr character::Berth GetBerth() const { return character::Berth(characterId); }
		constexpr character::Messages GetMessages() const { return character::Messages(characterId); }
		constexpr character::Items GetItems() const { return character::Items(characterId); }
		constexpr character::Counters GetCounters() const { return character::Counters(characterId); }
		constexpr character::Flags GetFlags() const { return character::Flags(characterId); }
		constexpr character::Marks GetMarks() const { return character::Marks(characterId); }
		constexpr character::Skills GetSkills() const { return character::Skills(characterId); }
		constexpr character::Deliveries GetDeliveries() const { return character::Deliveries(characterId); }

		bool IsDead() const;
		std::string GetName() const;
		void ApplyTurnEffects() const;
		Island GetOriginIsland() const;
		Tribe GetTribe() const;
		bool IsPlayer() const;
		void SetIsland(Island island) const;
		void ClearIsland() const;
		std::optional<Island> TryGetIsland() const;
		bool IsOnIsland() const;
		Island GetIsland() const;
		void AcceptDelivery(const Delivery& delivery) const;

	private:
		int characterId;
	};
}
