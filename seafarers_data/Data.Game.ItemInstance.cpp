#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.ItemInstance.h"
namespace data::game
{
	using namespace std::string_literals;
	static const std::string CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [ItemInstances]
		(
			[ItemInstanceId] INTEGER PRIMARY KEY AUTOINCREMENT, 
			[ItemSubtypeId] INT NOT NULL, 
			[Quantity] INT NOT NULL
		);)"s;
	static const std::string CREATE_ITEM = 
		R"(INSERT INTO [ItemInstances]
		(
			[ItemSubtypeId],
			[Quantity]
		) 
		VALUES({},{});)"s;
	static const std::string QUERY_ITEM_SUBTYPE = 
		R"(SELECT 
			[ItemSubtypeId] 
		FROM [ItemInstances] 
		WHERE 
			[ItemInstanceId]={};)"s;
	static const std::string QUERY_QUANTITY = 
		R"(SELECT 
			[Quantity] 
		FROM [ItemInstances] 
		WHERE 
			[ItemInstanceId]={};)"s;
	static const std::string UPDATE_QUANTITY = 
		R"(UPDATE [ItemInstances] 
		SET 
			[Quantity]={} 
		WHERE 
			[ItemInstanceId]={};)"s;
	static const std::string PURGE_ITEMS = 
		R"(DELETE FROM [ItemInstances] 
		WHERE 
			[Quantity]<=0;)"s;

	static const std::string FIELD_ITEM_SUBTYPE_ID = "ItemSubtypeId";
	static const std::string FIELD_QUANTITY = "Quantity";

	static const auto AutoCreateTable = Common::Run(CREATE_TABLE);

	int ItemInstance::Create(int itemSubtypeId, int quantity)
	{
		AutoCreateTable();
		Common::Execute(CREATE_ITEM, itemSubtypeId, quantity);
		return Common::LastInsertedIndex();
	}

	std::optional<int> ItemInstance::ReadItemSubtypeId(int itemInstanceId)
	{
		AutoCreateTable();
		auto records = Common::Execute(QUERY_ITEM_SUBTYPE, itemInstanceId);
		if (!records.empty())
		{
			return common::Data::ToOptionalInt(records.front()[FIELD_ITEM_SUBTYPE_ID]);
		}
		return std::nullopt;
	}

	std::optional<int> ItemInstance::ReadQuantity(int itemInstanceId)
	{
		AutoCreateTable();
		auto records = Common::Execute(QUERY_QUANTITY, itemInstanceId);
		if (!records.empty())
		{
			return common::Data::ToOptionalInt(records.front()[FIELD_QUANTITY]);
		}
		return std::nullopt;
	}

	void ItemInstance::WriteQuantity(int itemInstanceId, int quantity)
	{
		AutoCreateTable();
		Common::Execute(UPDATE_QUANTITY, quantity, itemInstanceId);
	}

	void ItemInstance::Purge()
	{
		AutoCreateTable();
		Common::Execute(PURGE_ITEMS);
	}
}