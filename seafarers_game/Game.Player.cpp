#include <Data.Game.Player.h>
#include "Game.Player.h"
namespace game
{
	int Player::GetCharacterId()
	{
		return data::game::Player::GetCharacterId().value();
	}
}