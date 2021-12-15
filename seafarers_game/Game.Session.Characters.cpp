#include "Game.Session.Characters.h"
#include "Game.Session.Character.h"
namespace game::session
{
	session::Character Characters::GetCharacter(int characterId) const
	{
		return Character(characterId);
	}
}