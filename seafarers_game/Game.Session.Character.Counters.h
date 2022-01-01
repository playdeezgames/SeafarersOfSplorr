#pragma once
#include "Game.Characters.Counter.h"
#include "Game.Session.Character.Counter.h"
namespace game::session::character
{
	struct Counters
	{
		constexpr explicit Counters(int characterId) : characterId(characterId) {}
		constexpr Counter GetCounter(const game::characters::Counter& counter) const 
		{
			return Counter(characterId, counter); 
		}
		void Initialize() const;
	private:
		int characterId;
	};
}
