#include "Game.Characters.h"
#include "Game.Player.h"
#include "Game.Session.h"
#include "Game.Session.Character.h"
namespace game
{
	std::optional<session::Character> Session::GetCharacter(int characterId) const
	{
		if (game::Characters::Exists(characterId))
		{
			return session::Character(characterId);
		}
		return std::nullopt;
	}

	std::optional<game::session::Character> Session::GetPlayerCharacter() const
	{
		return GetCharacter(game::Player::GetCharacterId());
	}

}