#include "Data.Game.StreetVendor.MenuItem.h"
#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Island.Feature.h"
namespace data::game::street_vendor
{
	using namespace std::string_view_literals;
	constexpr auto CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [StreetVendorMenuItems]
		(
			[StreetVendorMenuItemId] INTEGER PRIMARY KEY AUTOINCREMENT,
			[FeatureId] INT NOT NULL,
			[Price] INT NOT NULL,
			[Satiety] INT NOT NULL,
			[CookingSkill] INT NOT NULL
		);)"sv;
	constexpr auto INSERT_ITEM = 
		R"(INSERT INTO [StreetVendorMenuItems]
		(
			[FeatureId],
			[Price],
			[Satiety],
			[CookingSkill]
		) 
		VALUES({},{},{},{});)"sv;

	void MenuItem::Initialize()
	{
		island::Feature::Initialize();
		Common::Execute(CREATE_TABLE);
	}

	int MenuItem::Create(int featureId, int cost, int satiety, int cookingSkill)
	{
		Initialize();
		Common::Execute(INSERT_ITEM, featureId, cost, satiety, cookingSkill);
		return Common::LastInsertedIndex();
	}
}