#include <Data.Game.Character.Counter.h>
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

	void Counters::Write(int characterId, const Counter& counter, int value)
	{
		data::game::character::Counter::Write(characterId, (int)counter, value);
	}

	std::optional<int> Counters::Read(int characterId, const Counter& counter)
	{
		return data::game::character::Counter::Read(characterId, (int)counter);
	}
}