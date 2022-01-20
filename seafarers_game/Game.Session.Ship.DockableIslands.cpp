#include <algorithm>
#include <Data.Game.Island.h>
#include <Data.Game.Ship.h>
#include "Game.Session.h"
#include "Game.Session.World.h"
#include "Game.Session.Ship.DockableIslands.h"
namespace game::session::ship
{
	std::optional<Island> DockableIslands::TryGetFirst() const
	{
		auto maximumDistance = game::session::World().GetDistances().GetDock();
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