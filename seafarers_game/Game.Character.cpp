#include <Common.Data.h>
#include <Common.Heading.h>
#include <Common.NameGenerator.h>
#include <Common.RNG.h>
#include <Common.Utility.Optional.h>
#include <Data.Game.Character.h>
#include <Data.Game.Character.Rations.h>
#include "Game.Characters.h"
#include "Game.Characters.Flags.h"
#include "Game.Characters.Items.h"
#include "Game.Characters.Plights.h"
#include "Game.Characters.Ships.h"
#include "Game.Characters.Statistics.h"
#include "Game.Characters.State.h"
#include "Game.Items.h"
#include "Game.Player.h"
#include "Game.Ship.h"
#include "Game.ShipNames.h"
#include "Game.ShipTypes.h"
#include "Game.World.h"
namespace game
{
	static double DetermineHungerRate(int characterId)
	{
		double delta = -1.0;
		if (character::Plights::Has(characterId, character::Plight::DOUBLE_HUNGER) &&
			!character::Plights::Has(characterId, character::Plight::HUNGER_IMMUNITY))
		{
			delta *= 2.0;
		}
		if (character::Plights::Has(characterId, character::Plight::HUNGER_IMMUNITY) &&
			!character::Plights::Has(characterId, character::Plight::DOUBLE_HUNGER))
		{
			delta = 0.0;
		}
		return delta;
	}

	static void ApplyHunger(int characterId)
	{
		double delta = DetermineHungerRate(characterId);
		if (game::character::statistics::Satiety::IsStarving(characterId))
		{
			game::character::statistics::Health::Change(characterId, delta);
		}
		else
		{
			game::character::statistics::Satiety::Change(characterId, delta);
		}
	}

	static void ApplyEating(int characterId)
	{
		const double EAT_BENEFIT = 10.0;
		if (game::character::statistics::Satiety::NeedsToEat(characterId, EAT_BENEFIT))
		{
			const game::Item rationItem = game::Item::RATIONS;//TODO: when we can choose rations for an character, this will change
			auto rations = game::character::Items::Read(characterId, rationItem);
			if (rations > 0)
			{
				game::character::statistics::Satiety::Eat(characterId, EAT_BENEFIT);
				game::character::Items::Remove(characterId, rationItem, 1);
				if (game::character::Flags::Has(characterId, game::character::Flag::UNFED))
				{
					game::character::Flags::Clear(characterId, game::character::Flag::UNFED);
				}
				else
				{
					game::character::Flags::Write(characterId, game::character::Flag::FED);
				}
			}
			else
			{
				if (game::character::Flags::Has(characterId, game::character::Flag::FED))
				{
					game::character::Flags::Clear(characterId, game::character::Flag::FED);
				}
				else
				{
					game::character::Flags::Write(characterId, game::character::Flag::UNFED);
				}
			}
		}
	}

	static size_t DetermineTurnsSpent(int characterId)
	{
		size_t agingRate = 1;
		if (character::Plights::Has(characterId, character::Plight::DOUBLE_AGING) &&
			!character::Plights::Has(characterId, character::Plight::AGING_IMMUNITY))
		{
			agingRate = 2;
		}
		if (character::Plights::Has(characterId, character::Plight::AGING_IMMUNITY) &&
			!character::Plights::Has(characterId, character::Plight::DOUBLE_AGING))
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
			game::character::statistics::Turns::Change(characterId, -1);
			turnsSpent--;
		}
	}

	void Character::ApplyTurnEffects()
	{
		auto avatarIds = data::game::Character::All();
		for (auto characterId : avatarIds)
		{
			ApplyTurn(characterId);
			ApplyHunger(characterId);
			ApplyEating(characterId);
		}
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


	int Character::Create(const game::character::State& state)
	{
		data::game::Character data =
		{
			(int)state,
			GenerateName()
		};
		int characterId = data::game::Character::Create(data);
		GenerateCharacterRations(characterId);
		return characterId;
	}

	static void GenerateCharacterShip(int characterId)
	{
		auto worldSize = game::World::GetSize();
		auto shipType = game::ShipTypes::GenerateForAvatar();
		int shipId = game::Ship::Add({
			shipType,
			game::ShipNames::Generate(),
			{
				worldSize.GetX() / 2.0,
				worldSize.GetY() / 2.0
			},
			common::Data::ModuloDouble(
				common::RNG::FromRange(0.0, common::Heading::DEGREES),
				common::Heading::DEGREES).value(),
			1.0 });
		game::character::Ship::Write(characterId, shipId, BerthType::CAPTAIN);
	}

	void Character::Reset(const game::Difficulty&)
	{
		auto characterId = Create(game::character::State::AT_SEA);
		Player::Create(characterId);
		GenerateCharacterShip(characterId);
	}

	std::optional<std::string> Character::GetName(int characterId)
	{
		return common::utility::Optional::Map<data::game::Character, std::string>(
			data::game::Character::Read(characterId),
			[](const data::game::Character& character)
			{ return character.name; });
	}

	std::optional<character::State> Character::GetState(int characterId)
	{
		return common::utility::Optional::Bind<data::game::Character, character::State>(
			data::game::Character::Read(characterId),
			[](const data::game::Character& character) { return (character::State)character.state; });
	}

	void Character::SetState(int characterId, const character::State& state)
	{
		auto avatar = data::game::Character::Read(characterId).value();
		avatar.state = (int)state;
		data::game::Character::Write(characterId, avatar);
	}
}