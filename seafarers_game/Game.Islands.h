#pragma once
#include <Common.XY.h>
#include "Game.Difficulty.h"
#include <list>
#include <optional>
#include <string>
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
	std::list<IslandModel> GetCareeningIslands();
	bool CanDock();
	bool CanCareen();
	std::list<IslandModel> GetKnownIslands();
	void AddVisit(const common::XY<double>&, const int&);
	void SetKnown(const common::XY<double>&, const int&);
	std::optional<IslandModel> Read(const common::XY<double>&);
	const std::string UNKNOWN = "????";
}
