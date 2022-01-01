#pragma once
#include "Game.Characters.Flag.h"
#include <string>
namespace game::session::character
{
	struct Flag
	{
		constexpr explicit Flag(int characterId, const game::characters::Flag& flag)
			: characterId(characterId)
			, flag(flag) {}
		bool Has() const;
		void Set() const;
		void Reset() const;
		const std::string& GetName() const;
	private:
		int characterId;
		game::characters::Flag flag;
	};
}
