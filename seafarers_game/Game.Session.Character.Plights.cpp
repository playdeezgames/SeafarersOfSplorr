#include <algorithm>
#include <Data.Game.Character.Plight.h>
#include "Game.Session.Character.Plights.h"
#include <iterator>
namespace game::session::character
{
	using PlightData = data::game::character::Plight;
	std::vector<Plight> Plights::GetAll() const
	{
		auto plights = PlightData::All(characterId);
		std::vector<Plight> result;
		std::transform(
			plights.begin(), 
			plights.end(), 
			std::back_inserter(result), 
			[this](int plight) 
			{
				return Plight(characterId, (game::characters::Plight)plight);
			});
		return result;
	}

	void Plights::ApplyTurnEffects() const
	{
		auto plights = GetAll();
		std::for_each(
			plights.begin(), 
			plights.end(), 
			[](const Plight& plight) 
			{
				plight.ApplyTurnEffects();
			});
	}

	bool Plights::Has(const game::characters::Plight& plight) const
	{
		auto duration = PlightData::ReadDuration(characterId, (int)plight);
		return (duration.has_value() && duration.value() > 0);
	}
}