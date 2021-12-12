#include "Game.Characters.Messages.h"
#include "Game.Session.Character.Messages.h"
namespace game::session::character
{
	Messages::Messages(int characterId)
		: characterId(characterId)
	{

	}

	void Messages::Add(const std::string& color, const std::string& text) const
	{
		game::characters::Messages::Add(characterId, color, text);
	}
}