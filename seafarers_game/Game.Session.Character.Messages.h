#pragma once
#include "Game.Characters.Message.h"
#include <string>
namespace game::session::character
{
	struct Messages
	{
		constexpr Messages(int characterId) : characterId(characterId) {}
		void Add(const std::string& color, const std::string& text) const;
		characters::MessageList GetAll() const;
		void Clear() const;
	private:
		int characterId;
	};
}
