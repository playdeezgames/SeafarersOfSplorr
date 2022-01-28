#include "Game.Session.Character.Characteristics.h"
#include "Game.Session.Character.Counters.h"
#include "Game.Session.Character.h"
#include "Game.Session.Character.HitPoints.h"
#include "Game.Colors.h"
#include "Game.Session.Character.Deliveries.h"
#include "Game.Session.Character.ItemType.h"
#include "Game.Session.Character.KnownIslands.h"
#include "Game.Session.Character.Messages.h"
#include "Game.Session.Character.Plights.h"
#include "Game.Session.Characters.h"
#include "Game.Session.Item.Type.h"
#include <Common.RNG.h>
#include <Data.Game.Character.Delivery.h>
#include <Data.Game.Character.h>
#include <Data.Game.Character.Island.Current.h>
#include <Data.Game.Delivery.h>
#include <Data.Game.Feature.Delivery.h>
#include <Data.Game.Item.Type.Property.h>
#include <Data.Game.Player.h>
#include "Game.Session.Character.Berth.h"
namespace game::session
{
	bool Character::IsDead() const
	{
		return character::HitPoints(characterId).GetCurrent() <= 0;
	}

	void Character::SetIslandId(int islandId) const
	{
		data::game::character::island::Current::Write(characterId, islandId);
	}

	std::string Character::GetName() const
	{
		return data::game::Character::ReadName(characterId).value();
	}

	static size_t DetermineTurnsSpent(int characterId)
	{
		auto plights = game::session::character::Plights(characterId);
		size_t agingRate = 1;
		if (plights.Has(characters::Plight::DOUBLE_AGING) &&
			!plights.Has(characters::Plight::AGING_IMMUNITY))
		{
			agingRate = 2;
		}
		if (plights.Has(characters::Plight::AGING_IMMUNITY) &&
			!plights.Has(characters::Plight::DOUBLE_AGING))
		{
			agingRate = 0;
		}
		return agingRate;
	}

	static void ApplyTurn(int characterId)
	{
		auto turnsSpent = DetermineTurnsSpent(characterId);
		while (turnsSpent)
		{
			game::session::character::Counters(characterId)
				.GetCounter(game::characters::Counter::TURNS_REMAINING)
				.Change(-1);
			turnsSpent--;
		}
	}

	static void SufferWoundDueToStarvation(int characterId)
	{
		constexpr auto WOUND_INCREASE = 1;
		auto character =
			Characters()
			.GetCharacter(characterId);
		character::Messages(characterId).Add(game::Colors::RED, "{} loses HP dues to starvation!", character.GetName());
		auto counters =
			game::session::character::Counters(characterId);
		counters
			.GetCounter(game::characters::Counter::STARVATION)
			.Reset();
		counters
			.GetCounter(game::characters::Counter::WOUNDS)
			.Change(WOUND_INCREASE);
	}

	static void SufferStarvationDueToHunger(int characterId)
	{
		constexpr auto STARVATION_INCREASE = 1;
		auto character =
			Characters()
			.GetCharacter(characterId);
		character::Messages(characterId).Add(game::Colors::YELLOW, "{} is starving!", character.GetName());
		character::Characteristics(character.ToId())
			.GetCharacteristic(Characteristic::CONSTITUTION)
			.OnOpposedCheck(
				game::session::character::Counters(characterId)
					.GetCounter(game::characters::Counter::STARVATION)
					.Change(STARVATION_INCREASE),
				[characterId](bool success)
				{
					if (!success)
					{
						SufferWoundDueToStarvation(characterId);
					}
				});
	}

	static void ReduceSatiety(int characterId)
	{
		constexpr auto SATIETY_DELTA = -1;
		auto satiety = character::Counter(characterId, game::characters::Counter::SATIETY);
		if (satiety.GetValue() > 0)
		{
			satiety.Change(SATIETY_DELTA);
		}
		else
		{
			SufferStarvationDueToHunger(characterId);
		}
	}

	static void SufferHunger(int characterId)
	{
		character::Characteristics(characterId)
			.GetCharacteristic(Characteristic::CONSTITUTION)
			.OnCheck(
				[characterId](bool success)
				{
					if (!success)
					{
						ReduceSatiety(characterId);
					}
				});
	}

	void Character::ApplyTurnEffects() const
	{
		SufferHunger(characterId);
		ApplyTurn(characterId);
		game::session::character::Plights(characterId).ApplyTurnEffects();
		game::session::character::Deliveries(characterId).ApplyTurnEffects();
	}

	void Character::ClearIslandId() const
	{
		data::game::character::island::Current::Clear(characterId);
	}

	std::optional<int> Character::TryGetIslandId() const
	{
		return data::game::character::island::Current::Read(characterId);
	}

	bool Character::IsOnIsland() const
	{
		return TryGetIslandId().has_value();
	}

	int Character::GetIslandId() const
	{
		return TryGetIslandId().value();
	}

	int Character::GetOriginIslandId() const
	{
		return data::game::Character::ReadOriginIslandId(characterId).value();
	}

	int Character::GetTribeId() const
	{
		return data::game::Character::ReadTribeId(characterId).value();
	}

	bool Character::IsPlayer() const
	{
		return data::game::Player::GetCharacterId() == characterId;
	}

	void Character::AcceptDelivery(int deliveryId) const
	{
		character::KnownIslands(characterId)
			.AddKnownIsland(
					data::game::Delivery::ReadToIsland(deliveryId).value());
		data::game::feature::Delivery::Clear(deliveryId);
		data::game::character::Delivery::Create(characterId, deliveryId);
		character::Messages(characterId).Add(game::Colors::GREEN, "You accept the delivery.");
	}

	using PropertyData = data::game::item::type::Property;

	bool Character::Eat(int itemTypeId) const
	{
		auto characterItem = game::session::character::ItemType(characterId, itemTypeId);
		if (characterItem.HasAny())
		{
			auto itemType = game::session::item::Type(characterItem.GetItemType());
			if (itemType.HasCategory(game::item::Category::FOOD))
			{
				characterItem.RemoveQuantity(1);
				auto cookingSkill = PropertyData::ReadInt(itemTypeId, "cookingSkill").value();
				auto satiety = PropertyData::ReadInt(itemTypeId, "satiety").value();
				if (common::RNG::Roll<100>() <= cookingSkill)
				{
					auto counter = game::session::character::Counter(characterId, game::characters::Counter::SATIETY);
					counter.Change(satiety);
					game::session::character::Messages(characterId).Add(game::Colors::GREEN, "Delicious!");
				}
				else
				{
					game::session::character::Plights(characterId).Inflict(game::characters::Plight::FOOD_POISONING, satiety);
					game::session::character::Messages(characterId).Add(game::Colors::RED, "You got food poisoning!");
				}
				return true;
			}
		}
		return false;
	}

	int Character::GetShipId() const
	{
		return game::session::character::Berth(characterId).GetShipId();
	}
}