#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Item.Type.h"
#include "Data.Game.Item.h"
namespace data::game
{
	using namespace std::string_literals;
	static const std::string CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [Items]
		(
			[ItemId] INTEGER PRIMARY KEY AUTOINCREMENT, 
			[ItemTypeId] INT NOT NULL, 
			[Quantity] INT NOT NULL,
			FOREIGN KEY ([ItemTypeId]) REFERENCES [ItemTypes]([ItemTypeId])
		);)"s;
	static const std::string CREATE_ITEM = 
		R"(INSERT INTO [Items]
		(
			[ItemTypeId],
			[Quantity]
		) 
		VALUES({},{});)"s;
	static const std::string QUERY_ITEM_COLUMN = 
		R"(SELECT 
			[{}] 
		FROM [Items] 
		WHERE 
			[ItemId]={};)"s;
	static const std::string UPDATE_QUANTITY = 
		R"(UPDATE [Items] 
		SET 
			[Quantity]={} 
		WHERE 
			[ItemId]={};)"s;
	static const std::string PURGE_ITEMS = 
		R"(DELETE FROM [Items] 
		WHERE 
			[Quantity]<=0;)"s;

	static const std::string FIELD_ITEM_TYPE_ID = "ItemTypeId";
	static const std::string FIELD_QUANTITY = "Quantity";

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