#pragma once
#include "Game.Characters.Counter.h"
namespace game::session::character
{
	struct Counter
	{
		constexpr explicit Counter(int characterId, const game::characters::Counter& counter)
			: characterId(characterId)
			, counter(counter) { }
		int GetValue() const;
		int Change(int delta) const;
		void Reset() const;
	private:
		int characterId;
		game::characters::Counter counter;
	};
}
