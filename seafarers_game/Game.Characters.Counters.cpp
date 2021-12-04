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
		{Counter::HUNGER, {"Hunger", 0}},
		{Counter::STARVATION, {"Starvation", 0}},
		{Counter::WOUNDS, {"Wounds", 0}}
	};

	const std::string& Counters::GetName(const Counter& counter)
	{
		return counterDescriptors.find(counter)->second.name;
	}

	void Counters::Write(int characterId, const Counter& counter, int value)
	{
		data::game::character::Counter::Write(characterId, (int)counter, value);
	}

	std::optional<int> Counters::Read(int characterId, const Counter& counter)
	{
		return data::game::character::Counter::Read(characterId, (int)counter);
	}

	void Counters::Initialize(int characterId)
	{
		for (auto descriptor : counterDescriptors)
		{
			if (descriptor.second.initialValue)
			{
				Write(characterId, descriptor.first, descriptor.second.initialValue.value());
			}
		}
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

		std::optional<int> Hunger::Change(int characterId, int delta)
		{
			return DoChange(characterId, Counter::HUNGER, delta);
		}

		void Hunger::Reset(int characterId)
		{
			Counters::Write(characterId, Counter::HUNGER, 0);
		}

		std::optional<int> Starvation::Change(int characterId, int delta)
		{
			return DoChange(characterId, Counter::STARVATION, delta);
		}

		void Starvation::Reset(int characterId)
		{
			Counters::Write(characterId, Counter::STARVATION, 0);
		}

		std::optional<int> Wounds::Change(int characterId, int delta)
		{
			return DoChange(characterId, Counter::WOUNDS, delta);
		}
	}
}