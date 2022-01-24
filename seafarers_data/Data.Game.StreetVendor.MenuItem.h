#pragma once
namespace data::game::street_vendor
{
	struct MenuItem
	{
		MenuItem() = delete;
		static void Initialize();
		static int Create(int featureId, int cost, int satiety, int cookingSkill);
	};
}