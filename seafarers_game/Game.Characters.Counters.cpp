#include "Game.Characters.Counters.h"
#include <map>
namespace game::characters
{
	static const std::map<Counter, std::string> counterNames =
	{
		{Counter::HUNGER, "Hunger"},
		{Counter::STARVATION, "Starvation"}
	};

	const std::string& Counters::GetName(const Counter& counter)
	{
		return counterNames.find(counter)->second;
	}
}