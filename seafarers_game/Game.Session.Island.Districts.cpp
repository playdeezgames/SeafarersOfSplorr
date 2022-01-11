#include <algorithm>
#include <Data.Game.Island.District.h>
#include "Game.Session.Island.Districts.h"
#include <iterator>
namespace game::session::island
{
	std::vector<District> Districts::GetDistricts() const
	{
		std::vector<District> result;
		auto districts = data::game::island::District::Read(islandId);
		std::transform(
			districts.begin(),
			districts.end(),
			std::back_inserter(result),
			[this](int district) 
			{
				return District(islandId,(game::island::District)district);
			});
		return result;
	}

	void Districts::Populate(const game::Difficulty& difficulty) const
	{

	}

}