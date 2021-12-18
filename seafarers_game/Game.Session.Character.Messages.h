#pragma once
#include "Game.Characters.Message.h"
#include <string>
namespace game::session::character
{
	struct Messages
	{
		Messages(int);
		void Add(const std::string&, const std::string&) const;
		characters::MessageList GetAll() const;
		void Clear() const;
	private:
		int characterId;
	};
}
