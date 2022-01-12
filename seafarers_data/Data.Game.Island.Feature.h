#pragma once
#include <string>
namespace data::game::island
{
	struct Feature
	{
		Feature() = delete;
		static void Initialize();
		static int Create(int islandId, const std::string& name, int featureType, int district);
	};
}