#include <algorithm>
#include <Data.Game.Island.h>
#include <Data.Game.Ship.h>
#include "Game.Session.h"
#include "Game.Session.Ship.VisibleIslands.h"
#include "Game.Session.World.h"
#include <iterator>
namespace game::session::ship
{
	std::vector<Island> VisibleIslands::GetAll() const
	{
		auto maximumDistance = game::session::World().GetDistances().GetView();//TODO: this is the same as DockableIslands, except for the max dist value
		auto shipLocation = data::game::Ship::GetLocation(shipId).value();
		auto islands = data::game::Island::All();
		auto last =
			std::partition(
				islands.begin(), 
				islands.end(),
				[shipLocation, maximumDistance](int islandId) 
				{
					auto location = data::game::Island::ReadLocation(islandId).value();
					auto delta = location - shipLocation;
					return (delta.GetMagnitude() <= maximumDistance);
				});
		std::vector<Island> result;
		std::transform(
			islands.begin(),
			last,
			std::back_inserter(result),
			[](int islandId) 
			{
				return Island(islandId);
			});
		return result;
	}

	bool VisibleIslands::HasAny() const
	{
		return !GetAll().empty();
	}

	int VisibleIslands::GetCount() const
	{
		return (int)GetAll().size();
	}
}