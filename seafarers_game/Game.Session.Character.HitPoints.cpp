#include "Game.Characters.Characteristics.h"
#include "Game.Characters.Counters.h"
#include "Game.Session.Character.HitPoints.h"
namespace game::session::character
{
	HitPoints::HitPoints(int characterId)
		: characterId(characterId)
	{
		
	}

	std::optional<int> HitPoints::GetMaximum() const
	{
		return game::characters::Characteristics::GetMaximumHitPoints(characterId);
	}

	std::optional<int> HitPoints::GetCurrent() const
	{
		auto maximum = GetMaximum();
		if (maximum)
		{
			auto value = maximum.value() - game::characters::Counters::Read(characterId, game::characters::Counter::WOUNDS).value_or(0);
			return value < 0 ? 0 : value;
		}
		return std::nullopt;
	}
}