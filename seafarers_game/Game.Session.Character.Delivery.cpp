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

	static void FinalizeDelivery(const Delivery& delivery, int reputationDelta)
	{
		int deliveryId = delivery.operator int();
		int characterId = data::game::character::Delivery::ReadCharacterId(deliveryId).value();
		ChangeCharacterIslandReputation(characterId, delivery.GetToIslandId(), reputationDelta);
		ChangeCharacterIslandReputation(characterId, delivery.GetFromIslandId(), reputationDelta);
		data::game::character::Delivery::Remove(deliveryId);
		data::game::Delivery::Remove(deliveryId);
	}

	void Delivery::Abandon() const
	{
		constexpr int REPUTATION_PENALTY = -1;
		//TODO: add character message
		FinalizeDelivery(*this, REPUTATION_PENALTY);
	}

	void Delivery::Complete() const
	{
		constexpr int REPUTATION_BONUS = 1;
		//TODO: add character message
		FinalizeDelivery(*this, REPUTATION_BONUS);
	}
}