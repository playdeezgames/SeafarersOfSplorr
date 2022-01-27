#pragma once
#include <optional>
#include <string>
#include <vector>
namespace data::game::street_vendor
{
	struct MenuItem
	{
		MenuItem() = delete;
		static void Initialize();
		static int Create(int featureId, int itemTypeId);
		static std::optional<int> ReadItemTypeId(int menuItemId);
		static std::vector<int> ReadForFeature(int featureId);
	};
}