#include <Common.NameGenerator.h>
#include <Common.RNG.h>
#include <Data.Game.Player.h>
#include <Data.Game.Character.h>
#include <Data.Game.Character.Characteristic.h>
#include <Data.Game.Character.Rations.h>
#include "Game.Items.h"
#include "Game.Characters.Characteristics.h"
#include "Game.Characters.Counters.h"
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
		game::Characters::Reset(difficulty);
		game::characters::Equipment::Reset(difficulty);
		game::characters::Plights::Reset(difficulty);
	}

	void Characters::ApplyTurnEffects() const
	{
		game::Characters::ApplyTurnEffects();
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
		characters::Counters::Initialize(characterId);
		return characterId;
	}
}