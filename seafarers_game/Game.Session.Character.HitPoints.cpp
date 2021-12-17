#include <Data.Game.Character.Characteristic.h>
#include "Game.Characteristic.h"
#include "Game.Characters.Counters.h"
#include "Game.Session.Character.HitPoints.h"
namespace game::session::character
{
	HitPoints::HitPoints(int characterId)
		: characterId(characterId)
	{
		
	}

	static std::optional<int> TryGetMaximum(int characterId)
	{
		auto constitution = data::game::character::Characteristic::Read(characterId, (int)Characteristic::CONSTITUTION);
		if (constitution)
		{
			auto size = data::game::character::Characteristic::Read(characterId, (int)Characteristic::SIZE);
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
			auto value = maximum.value() - game::characters::Counters::Read(characterId, game::characters::Counter::WOUNDS).value_or(0);
			return value < 0 ? 0 : value;
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
		auto wounds = game::characters::Counters::Read(characterId, game::characters::Counter::WOUNDS);
		if (wounds)
		{
			game::characters::Counters::Write(
				characterId,
				game::characters::Counter::WOUNDS,
				wounds.value()-delta);
		}
	}

}