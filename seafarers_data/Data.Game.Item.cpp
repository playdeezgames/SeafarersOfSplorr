#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Item.Type.h"
#include "Data.Game.Item.h"
namespace data::game
{
	using namespace std::string_view_literals;
	static constexpr std::string_view CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [Items]
		(
			[ItemId] INTEGER PRIMARY KEY AUTOINCREMENT, 
			[ItemTypeId] INT NOT NULL, 
			[Quantity] INT NOT NULL,
			FOREIGN KEY ([ItemTypeId]) REFERENCES [ItemTypes]([ItemTypeId])
		);)"sv;
	static constexpr std::string_view CREATE_ITEM =
		R"(INSERT INTO [Items]
		(
			[ItemTypeId],
			[Quantity]
		) 
		VALUES({},{});)"sv;
	static constexpr std::string_view QUERY_ITEM_COLUMN =
		R"(SELECT 
			[{}] 
		FROM [Items] 
		WHERE 
			[ItemId]={};)"sv;
	static constexpr std::string_view UPDATE_QUANTITY =
		R"(UPDATE [Items] 
		SET 
			[Quantity]={} 
		WHERE 
			[ItemId]={};)"sv;
	static constexpr std::string_view PURGE_ITEMS =
		R"(DELETE FROM [Items] 
		WHERE 
			[Quantity]<=0;)"sv;

	static constexpr std::string_view FIELD_ITEM_TYPE_ID = "ItemTypeId";
	static constexpr std::string_view FIELD_QUANTITY = "Quantity";

	void Item::Initialize()
	{
		item::Type::Initialize();
		Common::Execute(CREATE_TABLE);
	}

	int Item::Create(int itemTypeId, int quantity)
	{
		Initialize();
		Common::Execute(CREATE_ITEM, itemTypeId, quantity);
		return Common::LastInsertedIndex();
	}

	std::optional<int> Item::ReadItemTypeId(int itemId)
	{
		Initialize();
		return Common::TryToInt(
			Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_ITEM_TYPE_ID, itemId), 
			FIELD_ITEM_TYPE_ID);
	}

	std::optional<int> Item::ReadQuantity(int itemId)
	{
		Initialize();
		return Common::TryToInt(
			Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_QUANTITY, itemId),
			FIELD_QUANTITY);
		return std::nullopt;
	}

	void Item::WriteQuantity(int itemId, int quantity)
	{
		Initialize();
		Common::Execute(UPDATE_QUANTITY, quantity, itemId);
	}

	void Item::Purge()
	{
		Initialize();
		Common::Execute(PURGE_ITEMS);
	}
}