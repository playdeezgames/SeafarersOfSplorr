#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Item.Type.h"
#include "Data.Game.ItemInstance.h"
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
	static const std::string QUERY_ITEM_TYPE = 
		R"(SELECT 
			[ItemTypeId] 
		FROM [Items] 
		WHERE 
			[ItemId]={};)"s;
	static const std::string QUERY_QUANTITY = 
		R"(SELECT 
			[Quantity] 
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

	void ItemInstance::Initialize()
	{
		item::Type::Initialize();
		Common::Execute(CREATE_TABLE);
	}

	int ItemInstance::Create(int itemSubtypeId, int quantity)
	{
		Initialize();
		Common::Execute(CREATE_ITEM, itemSubtypeId, quantity);
		return Common::LastInsertedIndex();
	}

	std::optional<int> ItemInstance::ReadItemTypeId(int itemInstanceId)
	{
		Initialize();
		auto records = Common::Execute(QUERY_ITEM_TYPE, itemInstanceId);
		if (!records.empty())
		{
			return common::Data::ToOptionalInt(records.front()[FIELD_ITEM_TYPE_ID]);
		}
		return std::nullopt;
	}

	std::optional<int> ItemInstance::ReadQuantity(int itemInstanceId)
	{
		Initialize();
		auto records = Common::Execute(QUERY_QUANTITY, itemInstanceId);
		if (!records.empty())
		{
			return common::Data::ToOptionalInt(records.front()[FIELD_QUANTITY]);
		}
		return std::nullopt;
	}

	void ItemInstance::WriteQuantity(int itemInstanceId, int quantity)
	{
		Initialize();
		Common::Execute(UPDATE_QUANTITY, quantity, itemInstanceId);
	}

	void ItemInstance::Purge()
	{
		Initialize();
		Common::Execute(PURGE_ITEMS);
	}
}