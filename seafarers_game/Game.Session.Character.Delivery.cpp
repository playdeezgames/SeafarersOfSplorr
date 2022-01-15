#include <Data.Game.Character.Island.Reputation.h>
#include <Data.Game.Character.Delivery.h>
#include <Data.Game.Delivery.h>
#include "Game.Session.Character.Delivery.h"
namespace game::session::character
{
	int Delivery::GetToIslandId() const
	{
		return data::game::Delivery::ReadToIsland(deliveryId).value();
	}

	int Delivery::GetFromIslandId() const
	{
		return data::game::Delivery::ReadFromIsland(deliveryId).value();
	}

	static void ChangeCharacterIslandReputation(int characterId, int islandId, int delta)
	{
		using Reputation = data::game::character::island::Reputation;
		auto reputation = 
			Reputation::Read(characterId, islandId).value_or(0);
		Reputation::Write(characterId, islandId, reputation + delta);
	}

	void Delivery::Abandon() const
	{
		constexpr int REPUTATION_PENALTY = -1;
		int characterId = data::game::character::Delivery::ReadCharacterId(deliveryId).value();
		ChangeCharacterIslandReputation(characterId, GetToIslandId(), REPUTATION_PENALTY);
		ChangeCharacterIslandReputation(characterId, GetFromIslandId(), REPUTATION_PENALTY);
		data::game::character::Delivery::Remove(deliveryId);
		data::game::Delivery::Remove(deliveryId);
	}
}