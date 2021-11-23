#include <Data.Game.Player.h>
#include "Game.Player.h"
namespace game
{
	void Player::Create(int characterId)
	{
		data::game::Player::Create(characterId);
	}


	int Player::GetAvatarId()
	{
		return data::game::Player::GetAvatarId();
	}
}