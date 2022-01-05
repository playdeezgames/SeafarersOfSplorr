#pragma once
#include "Game.Session.Character.Flag.h"
#include <vector>
namespace game::session::character
{
	struct Flags
	{
		constexpr explicit Flags(int characterId) : characterId(characterId) {}
		constexpr Flag GetFlag(const game::characters::Flag& flag) const
		{
			return Flag(characterId, flag);
		}
		std::vector<Flag> GetAll() const;
	private:
		int characterId;
	};
}