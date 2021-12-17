#pragma once
#include "Game.Characters.Counter.h"
#include "Game.Session.Character.Counter.h"
namespace game::session::character
{
	struct Counters
	{
		Counters(int);
		Counter GetCounter(const game::characters::Counter&);
	private:
		int characterId;
	};
}
