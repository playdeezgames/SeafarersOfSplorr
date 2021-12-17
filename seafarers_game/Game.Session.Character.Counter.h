#pragma once
#include "Game.Characters.Counter.h"
namespace game::session::character
{
	struct Counter
	{
		Counter(int, const game::characters::Counter&);
		int Change(int) const;
	private:
		int characterId;
		game::characters::Counter counter;
	};
}
