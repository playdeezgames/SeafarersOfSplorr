#pragma once
#include <format>
#include "Game.Characters.Message.h"
#include <string>
namespace game::session::character
{
	struct Messages
	{
		constexpr explicit Messages(int characterId) : characterId(characterId) {}
		template<typename ...Ts>
		void Add(const std::string_view& color, const std::string_view& fmt, Ts... args)
		{
			AddInternal(color, std::format(fmt, args...));
		}
		characters::MessageList GetAll() const;
		void Clear() const;
	private:
		void AddInternal(const std::string_view& color, const std::string& text) const;
		int characterId;
	};
}
