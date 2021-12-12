#pragma once
#include <Common.XY.h>
#include "Game.Session.Fisheries.h"
#include "Game.Session.Islands.h"
#include "Game.Session.Ship.Berth.h"
#include <list>
#include <optional>
namespace game::session
{
	struct Ship
	{
		Ship(int);
		std::list<ship::Berth> GetBerths() const;
		std::optional<double> GetHeading() const;
		void SetHeading(double) const;
		std::optional<double> GetSpeed() const;
		void SetSpeed(double) const;
		Islands GetDockableIslands() const;
		Islands GetNearbyIslands() const;
		std::optional<common::XY<double>> GetLocation() const;
		Fisheries GetFisheries() const;
	private:
		int shipId;
	};
}
