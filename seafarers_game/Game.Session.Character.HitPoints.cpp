#include <Data.Game.Character.Characteristic.h>
#include <Data.Game.Character.Counter.h>
#include "Game.Characters.Counter.h"
#include "Game.Characteristic.h"
#include "Game.Session.Character.HitPoints.h"
namespace game::session::character
{
	using CounterData = data::game::character::Counter;
	using CharacteristicData = data::game::character::Characteristic;

	static std::optional<int> TryGetMaximum(int characterId)
	{
		auto constitution = CharacteristicData::Read(characterId, (int)Characteristic::CONSTITUTION);
		if (constitution)
		{
			auto size = CharacteristicData::Read(characterId, (int)Characteristic::SIZE);
			if (size)
			{
				return (constitution.value() + size.value() + 1) / 2;//CON+SIZ/2, round up!
			}
		}
		return std::nullopt;
	}

	static std::optional<int> TryGetCurrent(int characterId)
	{
		auto maximum = TryGetMaximum(characterId);
		if (maximum)
		{
			auto value = 
				maximum.value() 
				- CounterData::Read(characterId, (int)game::characters::Counter::WOUNDS).value_or(0);
			return std::max(value, 0);
		}
		return std::nullopt;
	}

	int HitPoints::GetMaximum() const
	{
		return TryGetMaximum(characterId).value();
	}

	int HitPoints::GetCurrent() const
	{
		return TryGetCurrent(characterId).value();
	}

	void HitPoints::Change(int delta) const
	{
		auto wounds = CounterData::Read(characterId, (int)game::characters::Counter::WOUNDS);
		if (wounds)
		{
			CounterData::Write(characterId, (int)game::characters::Counter::WOUNDS, wounds.value() - delta);
		}
	}

}