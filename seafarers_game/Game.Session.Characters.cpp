#include <Common.NameGenerator.h>
#include <Common.RNG.h>
#include <Data.Game.Player.h>
#include <Data.Game.Character.h>
#include <Data.Game.Character.Characteristic.h>
#include <Data.Game.Character.Rations.h>
#include "Game.Items.h"
#include "Game.Session.Character.h"
#include "Game.Session.Characters.h"
#include "Game.Characters.h" //FOR APPLY TURN EFFECTS
#include "Game.Characters.Equipment.h" //FOR RESET
#include "Game.Characters.Items.h" //FOR RESET
#include "Game.Characters.Plights.h" //FOR APPLY TURN EFFECTS
#include "Game.Characters.Statistics.h" //FOR RESET
namespace game::session
{
	session::Character Characters::GetCharacter(int characterId) const
	{
		return Character(characterId);
	}

	void Characters::Reset(const Difficulty& difficulty) const
	{
		game::characters::Equipment::Reset(difficulty);
		game::characters::Plights::Reset(difficulty);
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

	static void ApplyEating(int characterId)
	{
		const game::Item rationItem = game::Item::RATIONS;//TODO: when we can choose rations for an character, this will change
		auto rations = game::characters::Items::Read(characterId, rationItem);
		auto character = Characters().GetCharacter(characterId);
		if (rations > 0)
		{
			character.GetCounters().GetCounter(game::characters::Counter::STARVATION).Change(-1);
			game::characters::Items::Remove(characterId, rationItem, 1);
			if (character.GetFlags().GetFlag(game::characters::Flag::UNFED).Has())
			{
				character.GetFlags().GetFlag(game::characters::Flag::UNFED).Reset();
			}
			else
			{
				character.GetFlags().GetFlag(game::characters::Flag::FED).Set();
			}
		}
		else
		{
			SufferHunger(characterId);
			if (character.GetFlags().GetFlag(game::characters::Flag::FED).Has())
			{
				character.GetFlags().GetFlag(game::characters::Flag::FED).Reset();
			}
			else
			{
				character.GetFlags().GetFlag(game::characters::Flag::UNFED).Set();
			}
		}
	}


	static void ApplyHunger(int characterId)
	{
		Characters()
			.GetCharacter(characterId)
			.GetCharacteristics()
			.GetCharacteristic(Characteristic::CONSTITUTION)
			.OnCheck([characterId](bool success)
				{
					if (!success)
					{
						ApplyEating(characterId);
					}
				});
	}

	void Characters::ApplyTurnEffects() const
	{
		auto avatarIds = data::game::Character::All();
		for (auto characterId : avatarIds)
		{
			ApplyTurn(characterId);
			ApplyHunger(characterId);
		}
		game::characters::Plights::ApplyTurnEffects(data::game::Player::GetCharacterId().value());//should apply to all characters
	}

	const std::map<std::string, size_t> CONSONANT_GENERATOR =
	{
		{"t", 1},
		{"g", 1},
		{"r", 1},
		{"n", 1},
		{"b", 1}
	};
	const std::map<std::string, size_t> VOWEL_GENERATOR =
	{
		{"a", 1},
		{"e", 1},
		{"i", 1},
		{"o", 1},
		{"u", 1}
	};
	const std::map<size_t, size_t> LENGTH_GENERATOR =
	{
		{3, 1},
		{4, 3},
		{5, 9},
		{6, 27},
		{7, 9},
		{8, 3},
		{9, 1}
	};
	const std::map<bool, size_t> IS_VOWEL_GENERATOR =
	{
		{true, 1},
		{false, 1}
	};
	const common::NameGenerator nameGenerator =
	{
		LENGTH_GENERATOR,
		IS_VOWEL_GENERATOR,
		VOWEL_GENERATOR,
		CONSONANT_GENERATOR
	};

	static std::string GenerateName()
	{
		return nameGenerator.Generate();
	}

	static void GenerateCharacterRations(int characterId)
	{
		data::game::character::Rations::Write(
			characterId,
			(int)game::Items::GenerateRationsForAvatar());
	}

	static const std::map<int, size_t> _2d6plus6 =
	{
		{8,1},
		{9,2},
		{10,3},
		{11,4},
		{12,5},
		{13,6},
		{14,5},
		{15,4},
		{16,3},
		{17,2},
		{18,1}
	};

	static const std::map<int, size_t> _3d6 =
	{
		{3,1},
		{4,3},
		{5,6},
		{6,10},
		{7,15},
		{8,21},
		{9,25},
		{10,27},
		{11,27},
		{12,25},
		{13,21},
		{14,15},
		{15,10},
		{16,6},
		{17,3},
		{18,1}
	};

	static const std::map<Characteristic, std::map<int, size_t>> characteristicRolls =
	{
		{ Characteristic::CHARISMA, _3d6},
		{ Characteristic::CONSTITUTION, _3d6},
		{ Characteristic::DEXTERITY, _3d6},
		{ Characteristic::INTELLIGENCE, _2d6plus6},
		{ Characteristic::POWER, _3d6},
		{ Characteristic::SIZE, _2d6plus6},
		{ Characteristic::STRENGTH, _3d6},
	};

	Character Characters::Create(const game::characters::State& state)
	{
		data::game::Character data =
		{
			(int)state,
			GenerateName()
		};
		int characterId = data::game::Character::Create(data);
		GenerateCharacterRations(characterId);
		for (auto characteristicRoll : characteristicRolls)
		{
			data::game::character::Characteristic::Write(
				characterId,
				(int)characteristicRoll.first,
				common::RNG::FromGenerator(characteristicRoll.second));
		}
		GetCharacter(characterId).GetCounters().Initialize();
		return characterId;
	}
}