#pragma once
#include <Common.XY.h>
#include "Game.ShipType.h"
#include "Game.Session.Ship.Berths.h"
#include "Game.Session.Ship.DockableIslands.h"
#include "Game.Session.Ship.VisibleIslands.h"
#include <optional>
namespace game::session
{
	struct Ship
	{
		constexpr explicit Ship(int shipId) : shipId(shipId) {}
		constexpr ship::Berths GetBerths() const { return ship::Berths(shipId); }
		constexpr ship::DockableIslands GetDockableIslands() const { return ship::DockableIslands(shipId); }
		constexpr ship::VisibleIslands GetNearbyIslands() const { return ship::VisibleIslands(shipId); }

		double GetHeading() const;
		void SetHeading(double) const;
		double GetSpeed() const;
		void SetSpeed(double) const;
		common::XY<double> GetLocation() const;
		void SetName(const std::string& name) const;
		std::string GetName() const;
		bool CanDock() const;
		void Dock() const;
		bool Undock() const;
		std::optional<Island> TryGetIsland() const;
		bool IsDocked() const;
		Island GetIsland() const;
		game::ShipType GetShipType() const;
		void ApplyTurnEffects() const;
	private:
		int shipId;
	};
}
