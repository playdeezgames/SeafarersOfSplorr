#pragma once
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
		std::optional<double> GetSpeed() const;
	private:
		int shipId;
	};
}
