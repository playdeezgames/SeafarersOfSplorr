#pragma once
#include "Game.Session.Character.KnownIslands.h"
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
