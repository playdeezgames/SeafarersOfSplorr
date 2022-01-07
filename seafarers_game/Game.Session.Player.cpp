#include <Common.Data.h>
#include <Common.Heading.h>
#include <Common.RNG.h>
#include <Data.Game.Character.Ship.h>
#include <Data.Game.Player.h>
#include <Data.Game.Ship.h>
#include "Game.Session.h"
#include "Game.Session.Characters.h"
#include "Game.Session.Player.h"
#include "Game.ShipNames.h"
namespace game::session
{
	Character Player::GetCharacter() const
	{
		return Character(data::game::Player::GetCharacterId().value());
	}

	using ShipData = data::game::Ship;

	static void GenerateCharacterShip(int characterId)
	{
		auto worldSize = game::Session().GetWorld().GetBounds().GetSize();
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

	void Player::Populate(const Difficulty& difficulty) const
	{
		auto character = game::session::Characters().Create(game::characters::State::AT_SEA);
		data::game::Player::Create((int)character);
		GenerateCharacterShip((int)character);
	}

	void Player::Reset() const
	{
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

	static int crewDetailCharacterId = 0;

	void Player::SetCrewDetailCharacterId(int characterId)
	{
		crewDetailCharacterId = characterId;
	}

	int Player::GetCrewDetailCharacterId()
	{
		return crewDetailCharacterId;
	}

	static std::string renameShipAdjective = "";

	void Player::SetRenameShipAdjective(const std::string& adjective)
	{
		renameShipAdjective = adjective;
	}

	const std::string& Player::GetRenameShipAdjective()
	{
		return renameShipAdjective;
	}

	static std::string headForMarkName;

	void Player::SetHeadForMark(const std::string& markName)
	{
		headForMarkName = markName;
	}

	const std::string Player::GetHeadForMark()
	{
		return headForMarkName;
	}

	static game::Profession playerProfession = game::Profession::SAILOR;

	void Player::SetProfession(const game::Profession& profession)
	{
		playerProfession = profession;
	}

	game::Profession Player::GetProfession()
	{
		return playerProfession;
	}

	void Player::SetCharacterCreationDefaults()
	{
		SetProfession(game::Profession::SAILOR);
	}

	static std::vector<game::SkillCategory> electiveSkillCategories;

	void Player::ClearElectiveSkillCategories()
	{
		electiveSkillCategories.clear();
	}

}