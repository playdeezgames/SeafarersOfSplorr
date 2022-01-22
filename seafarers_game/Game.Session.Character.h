#pragma once
#include "Game.Session.Character.Berth.h"
#include "Game.Session.Character.Counters.h"
#include "Game.Session.Character.Deliveries.h"
#include "Game.Session.Character.KnownIslands.h"
#include "Game.Session.Character.ItemTypes.h"
#include "Game.Session.Character.Marks.h"
#include "Game.Session.Character.Plights.h"
#include "Game.Session.Island.h"
#include <optional>
namespace game::session
{
	struct Character
	{
		constexpr explicit Character(int characterId) : characterId(characterId) {}
		constexpr int ToId() const { return characterId; }
		constexpr static Character ToCharacter(int characterId) { return Character(characterId); }

		constexpr character::KnownIslands GetKnownIslands() const { return character::KnownIslands(characterId); }
		constexpr character::Plights GetPlights() const { return character::Plights(characterId); }
		constexpr character::Berth GetBerth() const { return character::Berth(characterId); }
		constexpr character::Counters GetCounters() const { return character::Counters(characterId); }
		constexpr character::Marks GetMarks() const { return character::Marks(characterId); }
		constexpr character::Deliveries GetDeliveries() const { return character::Deliveries(characterId); }
		constexpr character::ItemTypes GetItemTypes() const { return character::ItemTypes(characterId); }

		bool IsDead() const;
		std::string GetName() const;
		void ApplyTurnEffects() const;
		Island GetOriginIsland() const;
		int GetTribeId() const;
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
