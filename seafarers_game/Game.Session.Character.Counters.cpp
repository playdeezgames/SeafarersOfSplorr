#include <algorithm>
#include "Game.Session.Character.Counters.h"
#include <vector>
namespace game::session::character
{
	static const std::vector<game::characters::Counter> counters =
	{
		game::characters::Counter::STARVATION,
		game::characters::Counter::WOUNDS,
		game::characters::Counter::SATIETY,
		game::characters::Counter::TURNS_REMAINING
	};

	void Counters::Initialize() const
	{
		std::for_each(
			counters.begin(),
			counters.end(),
			[this](const game::characters::Counter& counter) 
			{
				GetCounter(counter).Reset();
			});
	}
}