#include <Common.Data.h>
#include <Common.Heading.h>
#include <Common.RNG.h>
#include <Data.Game.Player.h>
#include "Game.Characters.Items.h"
#include "Game.Characters.Ships.h"
#include "Game.Session.Characters.h"
#include "Game.Session.Player.h"
#include "Game.Session.World.Bounds.h"
#include "Game.Ship.h"
#include "Game.ShipNames.h"
#include "Game.ShipTypes.h"
#include "Game.Characters.Statistics.h"
namespace game::session
{
	Character Player::GetCharacter() const
	{
		return Character(data::game::Player::GetCharacterId().value());
	}

	static void GenerateCharacterShip(int characterId)
	{
		auto worldSize = world::Bounds().GetSize();
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
		game::characters::Ships::Write(characterId, shipId, BerthType::CAPTAIN);
	}

	void Player::Reset(const Difficulty& difficulty) const
	{
		auto character = game::session::Characters().Create(game::characters::State::AT_SEA);
		data::game::Player::Create(character.characterId);
		game::characters::Items::Reset(data::game::Player::GetCharacterId().value(), difficulty);
		game::characters::Statistics::Reset(data::game::Player::GetCharacterId().value(), difficulty);
		GenerateCharacterShip(character.characterId);
	}

	std::optional<Character> Player::TryGetCharacter() const
	{
		auto characterId = data::game::Player::GetCharacterId();
		if (characterId)
		{
			return Character(characterId.value());
		}
		return std::nullopt;
	}
}