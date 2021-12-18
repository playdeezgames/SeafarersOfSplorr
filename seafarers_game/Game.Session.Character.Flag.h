#pragma once
#include "Game.Characters.Flag.h"
namespace game::session::character
{
	struct Flag
	{
		Flag(int, const game::characters::Flag&);
		bool Has() const;
		void Set() const;
		void Reset() const;
	private:
		int characterId;
		game::characters::Flag flag;
	};
}
