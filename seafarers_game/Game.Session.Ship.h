#pragma once
#include <Common.XY.h>
#include "Game.ShipType.h"
#include <optional>
#include <string>
namespace game::session
{
	struct Ship
	{
		constexpr explicit Ship(int shipId) : shipId(shipId) {}
		constexpr int ToId() const { return shipId; }

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
		std::optional<int> TryGetIslandId() const;
		bool IsDocked() const;
		int GetIslandId() const;
		game::ShipType GetShipType() const;
		void ApplyTurnEffects() const;
	private:
		int shipId;
	};
}
