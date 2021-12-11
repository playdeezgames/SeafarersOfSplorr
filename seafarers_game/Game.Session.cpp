#include "Game.Session.h"
#include "Game.Session.Character.h"
#include "Game.Characters.h"
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
}