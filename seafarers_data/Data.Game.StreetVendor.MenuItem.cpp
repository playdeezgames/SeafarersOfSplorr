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
			[Name] TEXT NOT NULL,
			[Price] INT NOT NULL,
			[Satiety] INT NOT NULL,
			[CookingSkill] INT NOT NULL
		);)"sv;
	constexpr auto INSERT_ITEM = 
		R"(INSERT INTO [StreetVendorMenuItems]
		(
			[FeatureId],
			[Name],
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
			
	constexpr auto COLUMN_NAME = "Name"sv;
	constexpr auto COLUMN_PRICE = "Price"sv;
	constexpr auto COLUMN_STREET_VENDOR_MENU_ITEM_ID = "StreetVendorMenuItemId"sv;

	void MenuItem::Initialize()
	{
		island::Feature::Initialize();
		Common::Execute(CREATE_TABLE);
	}

	int MenuItem::Create(int featureId, const std::string& name, int cost, int satiety, int cookingSkill)
	{
		Initialize();
		Common::Execute(INSERT_ITEM, featureId, common::Data::QuoteString(name), cost, satiety, cookingSkill);
		return Common::LastInsertedIndex();
	}

	std::optional<std::string> MenuItem::ReadName(int menuItemId)
	{
		Initialize();
		return Common::TryToString(
			Common::TryExecuteForOne(QUERY_ITEM_COLUMN, COLUMN_NAME, menuItemId),
			COLUMN_NAME);
	}

	std::optional<int> MenuItem::ReadPrice(int menuItemId)
	{
		Initialize();
		return Common::TryToInt(
			Common::TryExecuteForOne(QUERY_ITEM_COLUMN, COLUMN_PRICE, menuItemId),
			COLUMN_PRICE);
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