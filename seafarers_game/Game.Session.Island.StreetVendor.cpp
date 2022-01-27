#include "Game.Session.Island.StreetVendor.h"
#include "Game.Item.Category.h"
#include "Game.Session.Item.Type.Commodity.h"
#include "Game.Session.Item.Type.Properties.h"
#include "Game.Session.Item.Types.h"
#include <algorithm>
#include <Common.RNG.h>
#include <Data.Game.StreetVendor.MenuItem.h>
#include <format>
#include <map>
namespace game::session::island
{
	static const std::map<size_t, size_t> foodMenuSizeGenerator =
	{
		{2, 2},
		{3, 1}
	};

	using namespace std::string_view_literals;

	static const std::vector<std::string_view> cookingStyles =
	{
		"pan-fried"sv,
		"boiled"sv,
		"baked"sv,
		"roasted"sv,
		"toasted"sv,
		"deep-fried"sv,
		"raw"sv,
		"seared"sv,
		"pickled"sv,
		"salted"sv,
		"dried"sv,
		"sauteed"sv,
		"poached"sv,
		"braised"sv
	};

	static const std::vector<std::string_view> foodItems =
	{
		"potatoes"sv,
		"clams"sv,
		"marmite"sv,
		"toad stools"sv,
		"rats"sv,
		"coconuts"sv,
		"kelp"sv,
		"fish"sv,
		"grubs"sv,
		"pinapples"sv,
		"lefsa"sv,
		"human"sv,
		"seagull"sv,
		"snails"sv
	};

	void StreetVendor::Populate(const game::Difficulty&) const
	{
		auto itemCount = common::RNG::FromGenerator(foodMenuSizeGenerator);
		int satiety = 5;
		double commodityAmount = 1.0;
		while (itemCount > 0)
		{
			auto cookingSkill = common::RNG::FromRange(80, 100);
			auto itemName = std::format("{} {}",
				common::RNG::FromVector(cookingStyles).value(),
				common::RNG::FromVector(foodItems).value());
			auto itemType = game::session::item::Types().Create(game::item::Category::FOOD, itemName);
			auto properties = game::session::item::type::Properties(itemType.operator int());
			properties.AddProperty("cookingSkill", cookingSkill);
			properties.AddProperty("satiety", satiety);
			auto commodity = game::session::item::type::Commodity(itemType.operator int(), game::Commodity::FOOD);
			commodity.SetAmount(commodityAmount);
			data::game::street_vendor::MenuItem::Create(
				featureId, 
				itemType.operator int());
			satiety *= 2;
			commodityAmount *= 2.0;
			itemCount--;
		}
	}

	void StreetVendor::ApplyTurnEffects() const
	{
		//deliberately blank... maybe change the menu from time to time?
	}
}