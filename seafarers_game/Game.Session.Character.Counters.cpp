#include "Game.Session.Character.Counters.h"
namespace game::session::character
{
	Counters::Counters(int characterId)
		: characterId(characterId)
	{

	}

	Counter Counters::GetCounter(const game::characters::Counter& counter)
	{
		return Counter(characterId, counter);
	}
}