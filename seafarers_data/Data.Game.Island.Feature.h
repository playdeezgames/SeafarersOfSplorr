#pragma once
#include <optional>
#include <string>
#include <vector>
namespace data::game::island
{
	struct Feature
	{
		Feature() = delete;
		static void Initialize();
		static int Create(int islandId, const std::string& name, int featureType, int district);
		static std::vector<int> ReadForIslandDistrict(int islandId, int district);
		static std::optional<std::string> ReadName(int featureId);
	};
}