#include <Data.Game.Character.h>
#include <Data.Game.Character.CurrentIsland.h>
#include <Data.Game.Character.Ship.h>
#include <Data.Game.Player.h>
#include <Data.Game.Ship.CurrentIsland.h>
#include "Game.Session.h"
#include "Game.Session.Character.h"
#include <map>
namespace game::session
{
	bool Character::IsDead() const
	{
		return GetHitpoints().GetCurrent() <= 0;
	}

	void Character::SetIsland(Island island) const
	{
		data::game::character::CurrentIsland::Write(characterId, (int)island);
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
		auto counters =
			Characters()
			.GetCharacter(characterId)
			.GetCounters();
		counters
			.GetCounter(game::characters::Counter::STARVATION)
			.Reset();
		counters
			.GetCounter(game::characters::Counter::WOUNDS)
			.Change(1);
	}

	static void SufferStarvationDueToHunger(int characterId)
	{
		Characters()
			.GetCharacter(characterId)
			.GetCharacteristics()
			.GetCharacteristic(Characteristic::CONSTITUTION)
			.OnOpposedCheck(
				Characters().GetCharacter(characterId).GetCounters().GetCounter(game::characters::Counter::STARVATION).Change(1),
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
		Characters()
			.GetCharacter(characterId)
			.GetCharacteristics()
			.GetCharacteristic(Characteristic::CONSTITUTION)
			.OnOpposedCheck(
				Characters().GetCharacter(characterId).GetCounters().GetCounter(game::characters::Counter::STARVATION).Change(1),
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
	}

	void Character::ClearIsland() const
	{
		data::game::character::CurrentIsland::Clear(characterId);
	}

	std::optional<Island> Character::TryGetIsland() const
	{
		auto islandId = data::game::character::CurrentIsland::Read(characterId);
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

	Tribe Character::GetTribe() const
	{
		return Tribe(data::game::Character::ReadTribeId(characterId).value());
	}

	bool Character::IsPlayer() const
	{
		return data::game::Player::GetCharacterId() == characterId;
	}
}