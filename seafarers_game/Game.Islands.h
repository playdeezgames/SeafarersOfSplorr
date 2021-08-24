#pragma once
#include <Common.XY.h>
#include "Game.Difficulty.h"
#include <list>
#include <optional>
#include <string>
namespace game
{
	struct Islands
	{
		struct IslandModel
		{
			common::XY<double> relativeLocation;
			common::XY<double> absoluteLocation;
			std::string name;
			std::optional<int> visits;
		};
		static void Reset(const game::Difficulty&);
		static std::list<IslandModel> GetViewableIslands();
		static std::list<IslandModel> GetDockableIslands();
		static std::list<IslandModel> GetCareeningIslands();
		static bool CanDock();
		static bool CanCareen();
		static std::list<IslandModel> GetKnownIslands();
		static void AddVisit(const common::XY<double>&, const int&);
		static void SetKnown(const common::XY<double>&, const int&);
		static std::optional<IslandModel> Read(const common::XY<double>&);
		static const std::string UNKNOWN;
	};
}
