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
		static int Create(int featureId, const std::string& name, int cost, int satiety, int cookingSkill);
		static std::optional<std::string> ReadName(int menuItemId);
		static std::optional<int> ReadPrice(int menuItemId);
		static std::optional<int> ReadCookingSkill(int menuItemId);
		static std::optional<int> ReadSatiety(int menuItemId);
		static std::vector<int> ReadForFeature(int featureId);
	};
}