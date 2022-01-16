#include <Data.Game.Character.Island.Reputation.h>
#include <Data.Game.Character.Delivery.h>
#include <Data.Game.Delivery.h>
#include "Game.Colors.h"
#include "Game.Session.Character.Delivery.h"
#include "Game.Session.Character.Messages.h"
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

	int Delivery::GetTimeLimit() const
	{
		return data::game::Delivery::ReadTimeLimit(deliveryId).value();
	}

	static void ChangeCharacterIslandReputation(int characterId, int islandId, int delta)
	{
		using Reputation = data::game::character::island::Reputation;
		auto reputation = 
			Reputation::Read(characterId, islandId).value_or(0);
		Reputation::Write(characterId, islandId, reputation + delta);
	}

	static void FinalizeDelivery(const Delivery& delivery, int reputationDelta, const std::string_view& messageColor, const std::string& messageText)
	{
		int deliveryId = delivery.operator int();
		int characterId = data::game::character::Delivery::ReadCharacterId(deliveryId).value();
		game::session::character::Messages(characterId).Add(messageColor, messageText);
		ChangeCharacterIslandReputation(characterId, delivery.GetToIslandId(), reputationDelta);
		ChangeCharacterIslandReputation(characterId, delivery.GetFromIslandId(), reputationDelta);
		data::game::character::Delivery::Remove(deliveryId);
		data::game::Delivery::Remove(deliveryId);
	}

	void Delivery::Abandon() const
	{
		constexpr int REPUTATION_PENALTY = -1;
		FinalizeDelivery(*this, REPUTATION_PENALTY, game::Colors::RED, "You abandon the delivery. Yer reputation suffers as a result.");
	}

	void Delivery::Complete() const
	{
		constexpr int REPUTATION_BONUS = 1;
		FinalizeDelivery(*this, REPUTATION_BONUS, game::Colors::GREEN, "You complete the delivery. Yer reputation improves as a result.");
	}

	void Delivery::ApplyTurnEffects() const
	{
		auto timeLimit = data::game::Delivery::ReadTimeLimit(deliveryId).value();
		data::game::Delivery::WriteTimeLimit(deliveryId, timeLimit - 1);
	}
}