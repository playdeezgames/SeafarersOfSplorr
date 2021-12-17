#include <Common.Utility.Table.h>
#include <Data.Game.Character.Counter.h>
#include "Game.Characters.Counters.h"
#include <map>
namespace game::characters
{
	struct CounterDescriptor
	{
		std::string name;
		std::optional<int> initialValue;
	};

	static const std::map<Counter, CounterDescriptor> counterDescriptors =
	{
		{Counter::STARVATION, {"Starvation", -10}},
		{Counter::WOUNDS, {"Wounds", 0}}
	};

	void Counters::Write(int characterId, const Counter& counter, int value)
	{
		data::game::character::Counter::Write(characterId, (int)counter, value);
	}

	std::optional<int> Counters::Read(int characterId, const Counter& counter)
	{
		return data::game::character::Counter::Read(characterId, (int)counter);
	}

	namespace counters
	{
		static std::optional<int> DoChange(int characterId, const Counter& counter, int delta)
		{
			auto counterValue = Counters::Read(characterId, counter);
			if (counterValue)
			{
				auto newValue = counterValue.value() + delta;
				Counters::Write(characterId, counter, newValue);
				return newValue;
			}
			return std::nullopt;
		}
	}
}