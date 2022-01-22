#include "Game.Session.Ship.DockableIslands.h"
#include "Game.Session.World.Distances.h"
#include "Game.Session.World.h"
#include <algorithm>
#include <Data.Game.Island.h>
#include <Data.Game.Ship.h>
namespace game::session::ship
{
	std::optional<Island> DockableIslands::TryGetFirst() const
	{
		auto maximumDistance = game::session::world::Distances::GetDock();
		auto shipLocation = data::game::Ship::GetLocation(shipId).value();
		auto islands = data::game::Island::All();
		auto iter = std::find_if(
			islands.begin(), 
			islands.end(), 
			[shipLocation, maximumDistance](int islandId) 
			{
				auto location = data::game::Island::ReadLocation(islandId).value();
				auto delta = location - shipLocation;
				return (delta.GetMagnitude() <= maximumDistance);
			});
		if (iter != islands.end())
		{
			return Island(*iter);
		}
		return std::nullopt;
	}
}