#pragma once
#include <string>
#include <optional>
namespace game::session
{
	struct Character
	{
		constexpr explicit Character(int characterId) : characterId(characterId) {}
		constexpr int ToId() const { return characterId; }
		constexpr static Character ToCharacter(int characterId) { return Character(characterId); }

		bool IsDead() const;
		std::string GetName() const;
		void ApplyTurnEffects() const;
		int GetOriginIslandId() const;
		int GetTribeId() const;
		bool IsPlayer() const;
		void SetIslandId(int islandId) const;
		void ClearIslandId() const;
		std::optional<int> TryGetIslandId() const;
		bool IsOnIsland() const;
		int GetIslandId() const;
		void AcceptDelivery(int deliveryId) const;
		bool Eat(int itemTypeId) const;

	private:
		int characterId;
	};
}
