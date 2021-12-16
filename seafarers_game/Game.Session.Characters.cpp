#include <Common.NameGenerator.h>
#include <Data.Game.Player.h>
#include <Data.Game.Character.h>
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

	Character Characters::Create(const game::characters::State& state)
	{
		data::game::Character data =
		{
			(int)state,
			GenerateName()
		};
		int characterId = data::game::Character::Create(data);
		GenerateCharacterRations(characterId);
		characters::Characteristics::GenerateLegacy(characterId);
		characters::Counters::Initialize(characterId);
		return characterId;
	}
}