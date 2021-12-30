#pragma once
#include <Common.XY.h>
#include "Game.Session.Ship.Berth.h"
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
		constexpr ship::DockableIslands GetDockableIslands() const { return ship::DockableIslands(shipId); }
		constexpr ship::VisibleIslands GetNearbyIslands() const { return ship::VisibleIslands(shipId); }
		common::XY<double> GetLocation() const;
		void SetName(const std::string&) const;
		std::string GetName() const;
	private:
		int shipId;
	};
}
