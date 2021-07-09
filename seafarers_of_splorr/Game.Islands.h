#pragma once
#include "Game.Difficulty.h"
#include <list>
#include "Common.XY.h"
#include <string>
#include <optional>
namespace game::Islands
{
	struct IslandModel
	{
		common::XY<double> relativeLocation;
		common::XY<double> absoluteLocation;
		std::string name;
		std::optional<int> visits;
	};
	void Reset(const game::Difficulty&);
	std::list<IslandModel> GetViewableIslands();
	std::list<IslandModel> GetDockableIslands();
	bool AddVisit(const common::XY<double>&, const int&);
}
