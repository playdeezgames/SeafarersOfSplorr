#include <Data.Game.Character.Delivery.h>
#include <Data.Game.Character.Island.Reputation.h>
#include <Data.Game.Character.ItemType.h>
#include <Data.Game.Delivery.h>
#include "Game.Colors.h"
#include "Game.Session.Character.Delivery.h"
#include "Game.Session.Character.Messages.h"
#include "Game.Session.Item.Type.h"
#include "Game.Session.World.h"
#include "Game.Session.Island.Markets.h"
namespace game::session::character
{
	using DeliveryData = data::game::Delivery;

	bool Delivery::IsLate() const
	{
		return GetTimeLimit() < 0;
	}

	static void ChangeCharacterIslandReputation(int characterId, int islandId, int delta)
	{
		using ReputationData = data::game::character::island::Reputation;
		auto reputation = 
			ReputationData::Read(characterId, islandId).value_or(0);
		ReputationData::Write(characterId, islandId, reputation + delta);
	}

	static void FinalizeDelivery(const Delivery& delivery, int reputationDelta, const std::string_view& messageColor, const std::string& messageText)
	{
		using CharacterDeliveryData = data::game::character::Delivery;
		auto deliveryId = delivery.operator int();
		auto characterId = CharacterDeliveryData::ReadCharacterId(deliveryId).value();
		auto messages = game::session::character::Messages(characterId);
		messages.Add<>(messageColor, messageText);
		ChangeCharacterIslandReputation(characterId, delivery.GetToIslandId(), reputationDelta);
		ChangeCharacterIslandReputation(characterId, delivery.GetFromIslandId(), reputationDelta);

		auto rewardValue = delivery.GetRewardValue();
		auto itemType = game::session::World::GetCurrencyItemSubtype().operator int();
		auto quantity = game::session::island::Markets(delivery.GetToIslandId()).GetMaximumPurchaseQuantity(itemType, rewardValue);
		quantity = std::max(quantity, 1);

		auto count = data::game::character::ItemType::Read(characterId, itemType).value_or(0);
		data::game::character::ItemType::Write(characterId, itemType, count + quantity);
		messages.Add(game::Colors::GREEN, "You receive {} {}.", quantity, game::session::item::Type(itemType).GetName());
		CharacterDeliveryData::Remove(deliveryId);
		DeliveryData::Remove(deliveryId);
	}

	void Delivery::Abandon() const
	{
		constexpr int REPUTATION_PENALTY = -1;
		FinalizeDelivery(
			*this, 
			REPUTATION_PENALTY, 
			game::Colors::RED, 
			"You abandon the delivery. Yer reputation suffers as a result.");
	}

	void Delivery::Complete() const
	{
		if (IsLate())
		{
			FinalizeDelivery(
				*this, 
				0, 
				game::Colors::GRAY, 
				"You complete the delivery.");
		}
		else
		{
			constexpr int REPUTATION_BONUS = 1;
			FinalizeDelivery(
				*this, 
				REPUTATION_BONUS, 
				game::Colors::GREEN, 
				"You complete the delivery. Yer reputation improves as a result.");
		}
	}

	void Delivery::ApplyTurnEffects() const
	{
		constexpr auto TIME_LIMIT_DELTA = -1;
		DeliveryData::WriteTimeLimit(
			this->operator int(), 
			DeliveryData::ReadTimeLimit(this->operator int()).value()
			+ TIME_LIMIT_DELTA);
	}
}