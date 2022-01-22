#include "Game.Session.Player.h"
#include "Game.Session.Characters.h"
#include "Game.Session.World.Bounds.h"
#include "Game.Session.World.h"
#include "Game.ShipNames.h"
#include <algorithm>
#include <Common.Data.h>
#include <Common.Heading.h>
#include <Common.RNG.h>
#include <Data.Game.Character.Ship.h>
#include <Data.Game.Player.h>
#include <Data.Game.Ship.h>
namespace game::session
{
	int Player::GetCharacterId()
	{
		return data::game::Player::GetCharacterId().value();
	}

	using ShipData = data::game::Ship;

	static void GenerateCharacterShip(int characterId)
	{
		auto worldSize = game::session::world::Bounds().GetSize();
		auto shipType = game::ShipType::RAFT;
		int shipId = ShipData::Create(
			(int)shipType,
			game::ShipNames::Generate(),
			{
				worldSize.GetX() / 2.0,
				worldSize.GetY() / 2.0
			},
			common::Data::ModuloDouble(
				common::RNG::FromRange(0.0, common::Heading::DEGREES),
				common::Heading::DEGREES).value(),
			1.0);
		data::game::character::Ship::Write(characterId, shipId, (int)BerthType::CAPTAIN);
	}

	void Player::Populate(const Difficulty&, const std::map<int, size_t> allocations)
	{
		auto character = game::session::Characters().Create();
		std::for_each(
			allocations.begin(),
			allocations.end(),
			[character](const std::pair<int, size_t>& entry)
			{
				auto skill = character.GetSkills().GetSkill(entry.first);
				skill.AllocatePoints(entry.second);
			});
		data::game::Player::Create((int)character);
		GenerateCharacterShip((int)character);
	}

	void Player::Reset()
	{
	}

	std::optional<int> Player::TryGetCharacterId()
	{
		return data::game::Player::GetCharacterId();
	}
}