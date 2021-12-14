#include "Game.Characters.Characteristics.h"
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
		return game::characters::Characteristics::GetMaximumHitPoints(characterId);
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