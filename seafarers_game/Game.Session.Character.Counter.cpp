#include <Common.Utility.Table.h>
#include <Data.Game.Character.Counter.h>
#include "Game.Session.Character.Counter.h"
#include <map>
#include <string>
namespace game::session::character
{
	using CounterData = data::game::character::Counter;

	int Counter::Change(int delta) const
	{
		auto newValue = GetValue() + delta;
		CounterData::Write(characterId, (int)counter, newValue);
		return newValue;
	}

	struct CounterDescriptor
	{
		std::string name;
		int initialValue;
	};

	static const std::map<game::characters::Counter, CounterDescriptor> counterDescriptors =
	{
		{ game::characters::Counter::STARVATION, {"Starvation", -10}},
		{ game::characters::Counter::WOUNDS, {"Wounds", 0}},
		{ game::characters::Counter::TURNS_REMAINING, {"Turns Remaining", 10000}}
	};


	int Counter::GetValue() const
	{
		auto counterValue = CounterData::Read(characterId, (int)counter);
		if (counterValue)
		{
			return counterValue.value();
		}
		return counterDescriptors.find(counter)->second.initialValue;
	}

	void Counter::Reset() const
	{
		auto descriptor = common::utility::Table::TryGetKey(counterDescriptors, counter);
		if (descriptor)
		{
			CounterData::Write(characterId, (int)counter, descriptor.value().initialValue);
		}
	}
}