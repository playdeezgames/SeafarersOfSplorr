#include "Data.Game.StreetVendor.MenuItem.h"
#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Island.Feature.h"
#include <algorithm>
#include <iterator>
namespace data::game::street_vendor
{
	using namespace std::string_view_literals;
	constexpr auto CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [StreetVendorMenuItems]
		(
			[StreetVendorMenuItemId] INTEGER PRIMARY KEY AUTOINCREMENT,
			[FeatureId] INT NOT NULL,
			[ItemTypeId] INT NOT NULL,
			[Price] INT NOT NULL,
			[Satiety] INT NOT NULL,
			[CookingSkill] INT NOT NULL
		);)"sv;
	constexpr auto INSERT_ITEM = 
		R"(INSERT INTO [StreetVendorMenuItems]
		(
			[FeatureId],
			[ItemTypeId],
			[Price],
			[Satiety],
			[CookingSkill]
		) 
		VALUES({},{},{},{},{});)"sv;
	constexpr auto QUERY_ITEM_COLUMN =
		R"(SELECT 
			[{}] 
		FROM [StreetVendorMenuItems] 
		WHERE 
			[StreetVendorMenuItemId]={};)"sv;
	constexpr auto QUERY_FOR_FEATURE = 
		R"(SELECT 
			[StreetVendorMenuItemId] 
		FROM [StreetVendorMenuItems] 
		WHERE 
			[FeatureId]={};)"sv;
			
	constexpr auto COLUMN_ITEM_TYPE_ID = "ItemTypeId"sv;
	constexpr auto COLUMN_PRICE = "Price"sv;
	constexpr auto COLUMN_STREET_VENDOR_MENU_ITEM_ID = "StreetVendorMenuItemId"sv;
	constexpr auto COLUMN_SATIETY = "Satiety"sv;
	constexpr auto COLUMN_COOKING_SKILL = "CookingSkill"sv;

	void MenuItem::Initialize()
	{
		island::Feature::Initialize();
		Common::Execute(CREATE_TABLE);
	}

	int MenuItem::Create(int featureId, int itemTypeId, int cost, int satiety, int cookingSkill)
	{
		Initialize();
		Common::Execute(INSERT_ITEM, featureId, itemTypeId, cost, satiety, cookingSkill);
		return Common::LastInsertedIndex();
	}

	std::optional<int> MenuItem::ReadPrice(int menuItemId)
	{
		Initialize();
		return Common::TryToInt(
			Common::TryExecuteForOne(QUERY_ITEM_COLUMN, COLUMN_PRICE, menuItemId),
			COLUMN_PRICE);
	}

	std::optional<int> MenuItem::ReadCookingSkill(int menuItemId)
	{
		Initialize();
		return Common::TryToInt(
			Common::TryExecuteForOne(QUERY_ITEM_COLUMN, COLUMN_COOKING_SKILL, menuItemId),
			COLUMN_COOKING_SKILL);
	}

	std::optional<int> MenuItem::ReadSatiety(int menuItemId)
	{
		Initialize();
		return Common::TryToInt(
			Common::TryExecuteForOne(QUERY_ITEM_COLUMN, COLUMN_SATIETY, menuItemId),
			COLUMN_SATIETY);
	}

	std::optional<int> MenuItem::ReadItemTypeId(int menuItemId)
	{
		Initialize();
		return Common::TryToInt(
			Common::TryExecuteForOne(QUERY_ITEM_COLUMN, COLUMN_ITEM_TYPE_ID, menuItemId),
			COLUMN_ITEM_TYPE_ID);
	}

	std::vector<int> MenuItem::ReadForFeature(int featureId)
	{
		Initialize();
		auto records = Common::Execute(QUERY_FOR_FEATURE, featureId);
		std::vector<int> result;
		std::transform(
			records.begin(), 
			records.end(), 
			std::back_inserter(result), 
			[](const auto& record) 
			{
				return Common::ToInt(record, COLUMN_STREET_VENDOR_MENU_ITEM_ID);
			});
		return result;
	}

}