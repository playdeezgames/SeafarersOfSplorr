#pragma once
#include "Game.Characters.Counter.h"
namespace game::session::character
{
	struct Counter
	{
		Counter(int, const game::characters::Counter&);
		int Change(int) const;
		void Reset() const;
	private:
		int characterId;
		game::characters::Counter counter;
	};
}
