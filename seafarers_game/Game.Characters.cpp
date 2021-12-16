#include <Common.Data.h>
#include <Common.Heading.h>
#include <Common.NameGenerator.h>
#include <Common.RNG.h>
#include <Common.Utility.Optional.h>
#include <Data.Game.Character.h>
#include <Data.Game.Character.Rations.h>
#include "Game.Characters.h"
#include "Game.Characters.Characteristics.h"
#include "Game.Characters.Counters.h"
#include "Game.Characters.Flags.h"
#include "Game.Characters.Items.h"
#include "Game.Characters.Plights.h"
#include "Game.Characters.Ships.h"
#include "Game.Characters.Statistics.h"
#include "Game.Characters.State.h"
#include "Game.Items.h"
#include "Game.Ship.h"
#include "Game.ShipNames.h"
#include "Game.ShipTypes.h"
namespace game
{
	static double DetermineHungerRate(int characterId)
	{
		double delta = -1.0;
		if (characters::Plights::Has(characterId, characters::Plight::DOUBLE_HUNGER) &&
			!characters::Plights::Has(characterId, characters::Plight::HUNGER_IMMUNITY))
		{
			delta *= 2.0;
		}
		if (characters::Plights::Has(characterId, characters::Plight::HUNGER_IMMUNITY) &&
			!characters::Plights::Has(characterId, characters::Plight::DOUBLE_HUNGER))
		{
			delta = 0.0;
		}
		return delta;
	}

	static size_t DetermineTurnsSpent(int characterId)
	{
		size_t agingRate = 1;
		if (characters::Plights::Has(characterId, characters::Plight::DOUBLE_AGING) &&
			!characters::Plights::Has(characterId, characters::Plight::AGING_IMMUNITY))
		{
			agingRate = 2;
		}
		if (characters::Plights::Has(characterId, characters::Plight::AGING_IMMUNITY) &&
			!characters::Plights::Has(characterId, characters::Plight::DOUBLE_AGING))
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
			game::characters::statistics::Turns::Change(characterId, -1);
			turnsSpent--;
		}
	}

	static void SufferWoundDueToStarvation(int characterId)
	{
		characters::counters::Starvation::Reset(characterId);
		characters::counters::Wounds::Change(characterId, 1);
	}

	static void SufferStarvationDueToHunger(int characterId)
	{
		characters::Characteristics::OnOpposedCheck(
			characterId,
			Characteristic::CONSTITUTION,
			characters::counters::Starvation::Change(characterId, 1).value(),
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
		characters::Characteristics::OnOpposedCheck(
			characterId,
			Characteristic::CONSTITUTION,
			characters::counters::Starvation::Change(characterId, 1).value(),
			[characterId](bool success)
			{
				if (!success)
				{
					SufferStarvationDueToHunger(characterId);
				}
			});
	}

	static void ApplyEating(int characterId)
	{
		const game::Item rationItem = game::Item::RATIONS;//TODO: when we can choose rations for an character, this will change
		auto rations = game::characters::Items::Read(characterId, rationItem);
		if (rations > 0)
		{
			characters::counters::Starvation::Change(characterId, -1);
			game::characters::Items::Remove(characterId, rationItem, 1);
			if (game::characters::Flags::Has(characterId, game::characters::Flag::UNFED))
			{
				game::characters::Flags::Clear(characterId, game::characters::Flag::UNFED);
			}
			else
			{
				game::characters::Flags::Write(characterId, game::characters::Flag::FED);
			}
		}
		else
		{
			SufferHunger(characterId);
			if (game::characters::Flags::Has(characterId, game::characters::Flag::FED))
			{
				game::characters::Flags::Clear(characterId, game::characters::Flag::FED);
			}
			else
			{
				game::characters::Flags::Write(characterId, game::characters::Flag::UNFED);
			}
		}
	}


	static void ApplyHunger(int characterId)
	{
		characters::Characteristics::OnCheck(
			characterId, 
			Characteristic::CONSTITUTION, 
			[characterId](bool success)
			{
				if (!success)
				{
					ApplyEating(characterId);
				}
			});
	}

	std::optional<std::string> Characters::GetName(int characterId)
	{
		return common::utility::Optional::Map<data::game::Character, std::string>(
			data::game::Character::Read(characterId),
			[](const data::game::Character& character)
			{ return character.name; });
	}

	std::optional<characters::State> Characters::GetState(int characterId)
	{
		return common::utility::Optional::Bind<data::game::Character, characters::State>(
			data::game::Character::Read(characterId),
			[](const data::game::Character& character) { return (characters::State)character.state; });
	}

	void Characters::SetState(int characterId, const characters::State& state)
	{
		auto avatar = data::game::Character::Read(characterId).value();
		avatar.state = (int)state;
		data::game::Character::Write(characterId, avatar);
	}
}