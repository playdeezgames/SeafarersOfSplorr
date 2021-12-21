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
		auto counterValue = data::game::character::Counter::Read(characterId, (int)counter).value();
		auto newValue = counterValue + delta;
		data::game::character::Counter::Write(characterId, (int)counter, newValue);
		return newValue;
	}

	int Counter::GetValue() const
	{
		return data::game::character::Counter::Read(characterId, (int)counter).value();
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
		{ game::characters::Counter::REPUTATION, {"Reputation", 0}},
		{ game::characters::Counter::INFAMY, {"Infamy", 0}}
	};

	void Counter::Reset() const
	{
		auto descriptor = common::utility::Table::TryGetKey(counterDescriptors, counter);
		if (descriptor)
		{
			data::game::character::Counter::Write(characterId, (int)counter, descriptor.value().initialValue.value());
		}
	}
}