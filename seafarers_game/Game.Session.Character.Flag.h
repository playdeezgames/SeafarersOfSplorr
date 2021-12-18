#pragma once
#include "Game.Characters.Flag.h"
#include <string>
namespace game::session::character
{
	struct Flag
	{
		Flag(int, const game::characters::Flag&);
		bool Has() const;
		void Set() const;
		void Reset() const;
		const std::string& GetName() const;
	private:
		int characterId;
		game::characters::Flag flag;
	};
}
