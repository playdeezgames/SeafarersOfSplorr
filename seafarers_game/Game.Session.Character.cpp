#include "Game.Characters.h"
#include "Game.Session.FishGame.h"
#include "Game.Characters.Characteristics.h"
#include "Game.Characters.Quests.h"
#include "Game.Characters.Ships.h"
#include "Game.Islands.h"
#include "Game.Session.Character.h"
namespace game::session
{
	Character::Character(int characterId)
		: characterId(characterId)
	{
	}

	std::list<game::session::character::Characteristic> Character::GetCharacteristics() const
	{
		std::list<game::session::character::Characteristic> result;
		auto characteristics = game::characters::Characteristics::Read(characterId);
		for (auto characteristic : characteristics)
		{
			result.push_back(character::Characteristic(characterId, characteristic.first));
		}
		return result;
	}

	character::HitPoints Character::GetHitpoints() const
	{
		return character::HitPoints(characterId);
	}

	static std::optional<character::Berth> TryGetBerth(int characterId)
	{
		if (characters::Ships::ReadShipId(characterId).has_value())
		{
			return character::Berth(characterId);
		}
		return std::nullopt;
	}

	character::Items Character::GetItems() const
	{
		return character::Items(characterId);
	}

	std::optional<character::Quest> Character::TryGetQuest() const
	{
		auto quest = game::characters::Quests::Read(characterId);
		if (quest)
		{
			return game::session::character::Quest(characterId);
		}
		return std::nullopt;
	}

	void Character::DoAction(const game::characters::Action& action) const
	{
		game::Characters::DoAction(characterId, action);
	}

	character::Messages Character::GetMessages() const
	{
		return character::Messages(characterId);
	}

	Islands Character::GetKnownIslands() const
	{
		auto islands = game::Islands::GetKnownIslands(characterId);
		std::list<int> result;
		for (auto island : islands)
		{
			result.push_back(island.id);
		}
		return Islands(result);
	}

	FishGame Character::GetFishGame() const
	{
		return game::session::FishGame(characterId);
	}

	character::Berth Character::GetBerth() const
	{
		return TryGetBerth(characterId).value();
	}

	character::Quest Character::GetQuest() const
	{
		return TryGetQuest().value();
	}
}