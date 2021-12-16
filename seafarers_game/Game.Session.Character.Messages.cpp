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

	characters::MessageList Messages::GetAll() const
	{
		return game::characters::Messages::Read(characterId);
	}

	void Messages::Clear() const
	{
		game::characters::Messages::Clear(characterId);
	}

}