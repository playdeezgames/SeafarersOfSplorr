#pragma once
#include <Common.XY.h>
#include <functional>
#include <map>
#include <optional>
#include <string>
#include <vector>
namespace data::game
{
	struct Island
	{
		static void Initialize();
		static int Create(const common::XY<double>& location, const std::string& name, int patronDemigodId);
		static std::optional<common::XY<double>> ReadLocation(int islandId);
		static std::optional<std::string> ReadName(int islandId);
		static std::optional<int> ReadPatronDemigodId(int islandId);
		static std::vector<int> All();
		static std::map<int, common::XY<double>> AllLocations();
		static void Clear();
	};

}
