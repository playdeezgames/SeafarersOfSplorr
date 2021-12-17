#include "Game.Session.Character.Counters.h"
#include <list>
namespace game::session::character
{
	Counters::Counters(int characterId)
		: characterId(characterId)
	{

	}

	Counter Counters::GetCounter(const game::characters::Counter& counter) const
	{
		return Counter(characterId, counter);
	}

	static const std::list<game::characters::Counter> counters = 
	{
		game::characters::Counter::STARVATION,
		game::characters::Counter::WOUNDS
	};

	void Counters::Initialize() const
	{
		for (auto counter : counters)
		{
			GetCounter(counter).Reset();
		}
	}
}