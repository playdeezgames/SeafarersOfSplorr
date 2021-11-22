#include <Common.Data.h>
#include <Common.Heading.h>
#include <Common.NameGenerator.h>
#include <Common.RNG.h>
#include <Common.Utility.h>
#include <Common.Utility.Optional.h>
#include <Data.Game.Character.h>
#include <Data.Game.Character.Rations.h>
#include <functional>
#include "Game.Character.h"
#include "Game.Character.Flags.h"
#include "Game.Character.Items.h"
#include "Game.Character.Plights.h"
#include "Game.Avatar.Ship.h"
#include "Game.Avatar.Statistics.h"
#include "Game.Character.StateTransition.h"
#include <Game.Items.h>
#include <Game.Player.h>
#include <Game.Ship.h>
#include <Game.ShipNames.h>
#include <Game.ShipTypes.h>
#include <Game.World.h>
#include <map>
namespace game
{
	static double DetermineHungerRate()
	{
		double delta = -1.0;
		if (character::Plights::Has(character::Plight::DOUBLE_HUNGER) &&
			!character::Plights::Has(character::Plight::HUNGER_IMMUNITY))
		{
			delta *= 2.0;
		}
		if (character::Plights::Has(character::Plight::HUNGER_IMMUNITY) &&
			!character::Plights::Has(character::Plight::DOUBLE_HUNGER))
		{
			delta = 0.0;
		}
		return delta;
	}

	static void ApplyHunger(int avatarId)
	{
		double delta = DetermineHungerRate();
		if (game::avatar::Statistics::IsStarving(avatarId))
		{
			game::avatar::Statistics::ChangeHealth(avatarId, delta);
		}
		else
		{
			game::avatar::Statistics::ChangeSatiety(avatarId, delta);
		}
	}

	static void ApplyEating(int avatarId)
	{
		const double EAT_BENEFIT = 10.0;
		if (game::avatar::Statistics::NeedToEat(avatarId, EAT_BENEFIT))
		{
			const game::Item rationItem = game::Item::RATIONS;//TODO: when we can choose rations for an avatar, this will change
			auto rations = game::character::Items::Read(avatarId, rationItem);
			if (rations > 0)
			{
				game::avatar::Statistics::Eat(avatarId, EAT_BENEFIT);
				game::character::Items::Remove(avatarId, rationItem, 1);
				if (game::character::Flags::Has(avatarId, game::character::Flag::UNFED))
				{
					game::character::Flags::Clear(avatarId, game::character::Flag::UNFED);
				}
				else
				{
					game::character::Flags::Write(avatarId, game::character::Flag::FED);
				}
			}
			else
			{
				if (game::character::Flags::Has(avatarId, game::character::Flag::FED))
				{
					game::character::Flags::Clear(avatarId, game::character::Flag::FED);
				}
				else
				{
					game::character::Flags::Write(avatarId, game::character::Flag::UNFED);
				}
			}
		}
	}

	static size_t DetermineTurnsSpent()
	{
		size_t agingRate = 1;
		if (character::Plights::Has(character::Plight::DOUBLE_AGING) &&
			!character::Plights::Has(character::Plight::AGING_IMMUNITY))
		{
			agingRate = 2;
		}
		if (character::Plights::Has(character::Plight::AGING_IMMUNITY) &&
			!character::Plights::Has(character::Plight::DOUBLE_AGING))
		{
			agingRate = 0;
		}
		return agingRate;
	}

	static void ApplyTurn(int avatarId)
	{
		auto turnsSpent = DetermineTurnsSpent();
		while (turnsSpent)
		{
			game::avatar::Statistics::SpendTurn(avatarId);
			turnsSpent--;
		}
	}

	void Character::ApplyTurnEffects()
	{
		auto avatarIds = data::game::Character::All();
		for (auto avatarId : avatarIds)
		{
			ApplyTurn(avatarId);
			ApplyHunger(avatarId);
			ApplyEating(avatarId);
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

	static void CreateAvatar(int avatarId)
	{
		data::game::Character data =
		{
			(int)game::character::State::AT_SEA,
			GenerateName()
		};
		data::game::Character::Write(avatarId, data);
	}

	static void GenerateAvatarRations(int avatarId)
	{
		data::game::character::Rations::Write(
			avatarId, 
			(int)game::Items::GenerateRationsForAvatar());
	}

	static void GenerateAvatarShip()
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
		game::avatar::Ship::Write(game::Player::GetAvatarId(), shipId, BerthType::CAPTAIN);
	}

	void Character::Reset(const game::Difficulty&, int avatarId)
	{
		CreateAvatar(avatarId);
		GenerateAvatarRations(avatarId);
		GenerateAvatarShip();
	}

	std::optional<std::string> Character::GetName(int avatarId)
	{
		return common::utility::Optional::Map<data::game::Character, std::string>(
			data::game::Character::Read(avatarId),
			[](const data::game::Character& avatar)
			{ return avatar.name; });
	}
}