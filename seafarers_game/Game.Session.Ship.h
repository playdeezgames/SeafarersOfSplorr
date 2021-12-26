#pragma once
#include <Common.XY.h>
#include "Game.Session.Fisheries.h"
#include "Game.Session.IslandsDeprecated.h"
#include "Game.Session.Ship.Berth.h"
#include "Game.Session.Ship.Fouling.h"
#include "Game.Session.Ship.DockableIslands.h"
#include "Game.Session.Ship.VisibleIslands.h"
#include <list>
#include <optional>
namespace game::session
{
	struct Ship
	{
		constexpr Ship(int shipId) : shipId(shipId) {}
		std::list<ship::Berth> GetBerths() const;
		double GetHeading() const;
		void SetHeading(double) const;
		double GetSpeed() const;
		void SetSpeed(double) const;
		const ship::DockableIslands GetDockableIslands() const { return ship::DockableIslands(shipId); }
		IslandDeprecated GetNearbyIslands() const;
		common::XY<double> GetLocation() const;
		Fisheries GetFisheries() const;
		ship::Fouling GetFouling(const game::Side&) const;
		void SetName(const std::string&) const;
		std::string GetName() const;
	private:
		int shipId;
	};
}
