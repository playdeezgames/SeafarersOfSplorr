#include <Common.Data.h>
#include <Common.Heading.h>
#include <Common.RNG.h>
#include "Game.Characters.h"
#include "Game.Characters.Items.h"
#include "Game.Characters.Ships.h"
#include "Game.Session.Player.h"
#include "Game.Ship.h"
#include "Game.ShipNames.h"
#include "Game.ShipTypes.h"
#include "Game.Characters.Statistics.h"
#include "Game.Player.h"
#include "Game.World.h"
namespace game::session
{
	Character Player::GetCharacter() const
	{
		return Character(game::Player::GetCharacterId());
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
		game::characters::Ships::Write(characterId, shipId, BerthType::CAPTAIN);
	}

	void Player::Reset(const Difficulty& difficulty) const
	{
		auto characterId = game::Characters::Create(game::characters::State::AT_SEA);
		game::Player::CreateLegacy(characterId);
		game::characters::Items::Reset(game::Player::GetCharacterId(), difficulty);
		game::characters::Statistics::Reset(game::Player::GetCharacterId(), difficulty);
		GenerateCharacterShip(characterId);
	}
}