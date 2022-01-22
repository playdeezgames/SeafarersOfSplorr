#include <Data.Game.Character.h>
#include <Data.Game.Character.Island.Current.h>
#include <Data.Game.Character.Delivery.h>
#include <Data.Game.Feature.Delivery.h>
#include <Data.Game.Player.h>
#include "Game.Colors.h"
#include "Game.Session.Character.h"
#include "Game.Session.Characters.h"
#include "Game.Session.Character.Characteristics.h"
#include "Game.Session.Character.HitPoints.h"
namespace game::session
{
	bool Character::IsDead() const
	{
		return character::HitPoints(characterId).GetCurrent() <= 0;
	}

	void Character::SetIsland(Island island) const
	{
		data::game::character::island::Current::Write(characterId, (int)island);
	}

	std::string Character::GetName() const
	{
		return data::game::Character::ReadName(characterId).value();
	}

	static size_t DetermineTurnsSpent(int characterId)
	{
		auto plights = Character::ToCharacter(characterId).GetPlights();
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
			Characters()
				.GetCharacter(characterId)
				.GetCounters()
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
		character.GetMessages().Add(game::Colors::RED, "{} loses HP dues to starvation!", character.GetName());
		auto counters =
			character
			.GetCounters();
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
		character.GetMessages().Add(game::Colors::YELLOW, "{} is starving!", character.GetName());
		character::Characteristics(character.ToId())
			.GetCharacteristic(Characteristic::CONSTITUTION)
			.OnOpposedCheck(
				character
					.GetCounters()
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

	static void SufferHunger(int characterId)
	{
		auto character = Characters()
			.GetCharacter(characterId);
		character::Characteristics(character.ToId())
			.GetCharacteristic(Characteristic::CONSTITUTION)
			.OnCheck(
				[characterId](bool success)
				{
					if (!success)
					{
						SufferStarvationDueToHunger(characterId);
					}
				});
	}

	void Character::ApplyTurnEffects() const
	{
		SufferHunger(characterId);
		ApplyTurn(characterId);
		GetPlights().ApplyTurnEffects();
		GetDeliveries().ApplyTurnEffects();
	}

	void Character::ClearIsland() const
	{
		data::game::character::island::Current::Clear(characterId);
	}

	std::optional<Island> Character::TryGetIsland() const
	{
		auto islandId = data::game::character::island::Current::Read(characterId);
		if (islandId)
		{
			return Island(islandId.value());
		}
		return std::nullopt;
	}

	bool Character::IsOnIsland() const
	{
		return TryGetIsland().has_value();
	}

	Island Character::GetIsland() const
	{
		return TryGetIsland().value();
	}

	Island Character::GetOriginIsland() const
	{
		return Island(data::game::Character::ReadOriginIslandId(characterId).value());
	}

	int Character::GetTribeId() const
	{
		return data::game::Character::ReadTribeId(characterId).value();
	}

	bool Character::IsPlayer() const
	{
		return data::game::Player::GetCharacterId() == characterId;
	}

	void Character::AcceptDelivery(const Delivery& delivery) const
	{
		GetKnownIslands().AddKnownIsland(game::session::Island(delivery.GetToIslandId()));
		data::game::feature::Delivery::Clear(delivery.operator int());
		data::game::character::Delivery::Create(characterId, delivery.operator int());
		GetMessages().Add(game::Colors::GREEN, "You accept the delivery.");
	}

}