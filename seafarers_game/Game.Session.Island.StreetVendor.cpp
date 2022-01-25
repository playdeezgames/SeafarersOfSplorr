#include "Game.Session.Island.StreetVendor.h"
#include <algorithm>
#include <map>
#include <Common.RNG.h>
#include <Data.Game.StreetVendor.MenuItem.h>
#include <format>
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
			data::game::street_vendor::MenuItem::Create(
				featureId, 
				std::format("{} {}", 
					common::RNG::FromVector(cookingStyles).value(), 
					common::RNG::FromVector(foodItems).value()),
				cost, 
				satiety, 
				cookingSkill);
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