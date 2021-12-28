#include <algorithm>
#include <Data.Game.Character.Plight.h>
#include "Game.Session.Character.Plights.h"
#include <iterator>
namespace game::session::character
{
	using PlightData = data::game::character::Plight;
	std::list<Plight> Plights::GetAll() const
	{
		auto plights = PlightData::All(characterId);
		std::list<Plight> result;
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
}