#include <algorithm>
#include <Common.NameGenerator.h>
#include <Common.RNG.h>
#include <Data.Game.Player.h>
#include <Data.Game.Character.h>
#include <Data.Game.Character.KnownIsland.h>
#include <Data.Game.Character.Plight.h>
#include <Data.Game.Character.Characteristic.h>
#include <Data.Game.Island.Tribe.h>
#include "Game.Session.Character.h"
#include "Game.Session.Characters.h"
#include "Game.Characters.Plights.h" //FOR APPLY TURN EFFECTS
#include <iterator>
namespace game::session
{
	session::Character Characters::GetCharacter(int characterId) const
	{
		return Character(characterId);
	}

	std::list<Character> Characters::GetAll() const
	{
		auto characters = data::game::Character::All();
		std::list<Character> result;
		std::transform(
			characters.begin(),
			characters.end(),
			std::back_inserter(result),
			Character::ToCharacter);
		return result;
	}

	void Characters::Reset() const
	{
		data::game::character::Plight::ClearAll();
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

	static void ApplyCharacterTurnEffects(const Character& character)
	{
		character.ApplyTurnEffects();
	}

	void Characters::ApplyTurnEffects() const
	{
		auto characters = GetAll();
		std::for_each(
			characters.begin(), 
			characters.end(), 
			ApplyCharacterTurnEffects);
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

	using IslandTribeData = data::game::island::Tribe;

	static int GenerateOriginIslandId()
	{
		return 
			common::RNG::FromGenerator(
				IslandTribeData::AllTotals());
	}

	static int GenerateTribeId(int islandId)
	{
		return 
			common::RNG::FromGenerator(
				IslandTribeData::All(islandId));
	}

	Character Characters::Create(const game::characters::State& state)
	{
		auto originIslandId = GenerateOriginIslandId();
		int characterId = 
			data::game::Character::Create(
				(int)state, 
				GenerateName(), 
				originIslandId, 
				GenerateTribeId(originIslandId));
		data::game::character::KnownIsland::Write(characterId, originIslandId);
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