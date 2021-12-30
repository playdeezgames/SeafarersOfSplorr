#pragma once
#include <Common.XY.h>
#include "Game.Session.Ship.Berths.h"
#include "Game.Session.Ship.DockableIslands.h"
#include "Game.Session.Ship.VisibleIslands.h"
#include "Game.Session.World.ShipType.h"
#include <list>
#include <optional>
namespace game::session
{
	struct Ship
	{
		constexpr Ship(int shipId) : shipId(shipId) {}
		const ship::Berths GetBerths() const { return ship::Berths(shipId); }
		double GetHeading() const;
		void SetHeading(double) const;
		double GetSpeed() const;
		void SetSpeed(double) const;
		constexpr ship::DockableIslands GetDockableIslands() const { return ship::DockableIslands(shipId); }
		constexpr ship::VisibleIslands GetNearbyIslands() const { return ship::VisibleIslands(shipId); }
		common::XY<double> GetLocation() const;
		void SetName(const std::string&) const;
		std::string GetName() const;
		bool CanDock() const;
		void Dock() const;
		bool Undock() const;
		std::optional<Island> TryGetIsland() const;
		Island GetIsland() const;
		world::ShipType GetShipType() const;
		void ApplyTurnEffects() const;
	private:
		int shipId;
	};
}
