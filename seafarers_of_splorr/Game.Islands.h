#pragma once
#include "Game.Difficulty.h"
#include <map>
#include "Common.XY.h"
#include <string>
#include <optional>
namespace game::Islands
{
	struct IslandModel
	{
		std::string name;
		std::optional<int> visits;
	};
	void Reset(const game::Difficulty&);
	std::map<common::XY<double>, IslandModel> GetViewableIslands();
}
