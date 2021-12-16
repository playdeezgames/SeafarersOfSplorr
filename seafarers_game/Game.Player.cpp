#include <Data.Game.Player.h>
#include "Game.Player.h"
namespace game
{
	int Player::GetCharacterId()
	{
		return TryGetCharacterId().value();
	}

	std::optional<int> Player::TryGetCharacterId()
	{
		return data::game::Player::GetCharacterId();
	}
}