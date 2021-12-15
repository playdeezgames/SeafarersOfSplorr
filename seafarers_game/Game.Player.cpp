#include <Data.Game.Player.h>
#include "Game.Player.h"
namespace game
{
	void Player::CreateLegacy(int characterId)
	{
		data::game::Player::Create(characterId);
	}

	int Player::GetCharacterId()
	{
		return TryGetCharacterId().value();
	}

	std::optional<int> Player::TryGetCharacterId()
	{
		return data::game::Player::GetCharacterId();
	}
}