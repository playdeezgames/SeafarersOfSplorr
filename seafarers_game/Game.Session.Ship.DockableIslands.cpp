#include <Data.Game.Island.h>
#include <Data.Game.Ship.h>
#include "Game.Session.h"
#include "Game.Session.Ship.DockableIslands.h"
namespace game::session::ship
{
	std::optional<Island> DockableIslands::TryGetFirst() const
	{
		auto maximumDistance = game::Session().GetWorld().GetDistances().GetDock();
		auto shipLocation = data::game::Ship::GetLocation(shipId).value();
		auto islands = data::game::Island::All();
		std::list<Island> result;
		for (auto island : islands)
		{
			auto delta = island.location - shipLocation;
			if (delta.GetMagnitude() <= maximumDistance)
			{
				return Island(island.id);
			}
		}
		return std::nullopt;
	}
}