#include <Common.Utility.Table.h>
#include <Data.Game.Character.Counter.h>
#include "Game.Session.Character.Counter.h"
#include <map>
#include <string>
namespace game::session::character
{
	Counter::Counter(int characterId, const game::characters::Counter& counter)
		: characterId(characterId)
		, counter(counter)
	{

	}

	int Counter::Change(int delta) const
	{
		auto counterValue = GetValue();
		auto newValue = counterValue + delta;
		data::game::character::Counter::Write(characterId, (int)counter, newValue);
		return newValue;
	}

	struct CounterDescriptor
	{
		std::string name;
		std::optional<int> initialValue;
	};

	static const std::map<game::characters::Counter, CounterDescriptor> counterDescriptors =
	{
		{ game::characters::Counter::STARVATION, {"Starvation", -10}},
		{ game::characters::Counter::WOUNDS, {"Wounds", 0}},
		{ game::characters::Counter::TURNS_REMAINING, {"Turns Remaining", 10000}}
	};

	int Counter::GetValue() const
	{
		auto counterValue = data::game::character::Counter::Read(characterId, (int)counter);
		if (counterValue)
		{
			return counterValue.value();
		}
		return counterDescriptors.find(counter)->second.initialValue.value_or(0);
	}

	void Counter::Reset() const
	{
		auto descriptor = common::utility::Table::TryGetKey(counterDescriptors, counter);
		if (descriptor)
		{
			data::game::character::Counter::Write(characterId, (int)counter, descriptor.value().initialValue.value());
		}
	}
}