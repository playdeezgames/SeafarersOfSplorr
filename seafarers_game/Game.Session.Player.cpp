#include "Game.Player.h"
#include "Game.Session.Player.h"
namespace game::session
{
	Character Player::GetCharacter() const
	{
		return Character(game::Player::GetCharacterId());
	}
}