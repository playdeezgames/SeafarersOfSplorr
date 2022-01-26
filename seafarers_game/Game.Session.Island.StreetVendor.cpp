#include "Game.Session.Island.StreetVendor.h"
#include "Game.Item.Category.h"
#include <algorithm>
#include <Common.RNG.h>
#include <Data.Game.StreetVendor.MenuItem.h>
#include <format>
#include <map>
#include "Game.Session.Item.Types.h"
#include "Game.Session.Item.Type.Properties.h"
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
		"dried"sv
	};

	static const std::vector<std::string_view> foodItems =
	{
		"potatos"sv,
		"clams"sv,
		"marmite"sv,
		"toad stools"sv,
		"rats"sv,
		"coconuts"sv,
		"kelp"sv,
		"fish"sv,
		"grubs"sv,
		"pinapples"sv,
		"lefsa"sv
	};

	void StreetVendor::Populate(const game::Difficulty&) const
	{
		auto itemCount = common::RNG::FromGenerator(foodMenuSizeGenerator);
		int cost = common::RNG::FromRange(1,3);
		int satiety = 5;
		while (itemCount > 0)
		{
			auto cookingSkill = common::RNG::FromRange(80, 100);
			auto itemName = std::format("{} {}",
				common::RNG::FromVector(cookingStyles).value(),
				common::RNG::FromVector(foodItems).value());
			//TODO: the item type is created here
			auto itemType = game::session::item::Types().Create(game::item::Category::FOOD, itemName);
			//TODO: set itemtype's satiety, and cooking skill
			auto properties = game::session::item::type::Properties(itemType.operator int());
			properties.AddProperty("cookingSkill", cookingSkill);
			properties.AddProperty("satiety", satiety);

			data::game::street_vendor::MenuItem::Create(
				featureId, 
				itemType.operator int(),
				cost);
			cost += common::RNG::FromRange(1, 3);
			satiety *= 2;
			itemCount--;
		}
	}

	void StreetVendor::ApplyTurnEffects() const
	{
		//deliberately blank... maybe change the menu from time to time?
	}
}