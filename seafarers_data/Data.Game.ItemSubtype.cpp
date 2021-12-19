#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.ItemSubtype.h"
namespace data::game
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [ItemSubtypes]([ItemSubtypeId] INTEGER PRIMARY KEY AUTOINCREMENT, [ItemTypeId] INT NOT NULL,[Subtype] INT NOT NULL, UNIQUE([ItemTypeId],[Subtype]));";
	static const std::string INSERT_ITEM = "INSERT INTO [ItemSubtypes]([ItemTypeId],[Subtype]) VALUES({},{});";
	static const std::string QUERY_MAXIMUM_SUBTYPE = "SELECT MAX([Subtype]) AS MaximumSubtype FROM [ItemSubtypes] WHERE [ItemTypeId]={};";
	static const std::string QUERY_ITEM_TYPE_ID = "SELECT [ItemTypeId] FROM [ItemSubtypes] WHERE [ItemSubtypeId]={};";
	static const std::string QUERY_SUBTYPE = "SELECT [Subtype] FROM [ItemSubtypes] WHERE [ItemSubtypeId]={};";
	static const std::string DELETE_ALL = "DELETE FROM [ItemSubtypes];";

	static const std::string FIELD_MAXIMUM_SUBTYPE = "MaximumSubtype";
	static const std::string FIELD_SUBTYPE = "Subtype";
	static const std::string FIELD_ITEM_TYPE_ID = "ItemTypeId";

	static const auto AutoCreateTable = Common::Run(CREATE_TABLE);

	int ItemSubtype::ReadNextSubtype(int itemTypeId)
	{
		AutoCreateTable();
		auto records = Common::Execute(QUERY_MAXIMUM_SUBTYPE, itemTypeId);
		if (!records.empty())
		{
			return common::Data::ToInt(records.front()[FIELD_MAXIMUM_SUBTYPE]) + 1;
		}
		return 0;
	}

	int ItemSubtype::Establish(int itemTypeId, int subtype)
	{
		AutoCreateTable();
		Common::Execute(INSERT_ITEM, itemTypeId, subtype);
		return Common::LastInsertedIndex();
	}

	std::optional<int> ItemSubtype::ReadItemType(int itemSubTypeId)
	{
		AutoCreateTable();
		auto records = Common::Execute(QUERY_ITEM_TYPE_ID, itemSubTypeId);
		if (!records.empty())
		{
			return common::Data::ToOptionalInt(records.front()[FIELD_ITEM_TYPE_ID]);
		}
		return std::nullopt;
	}

	std::optional<int> ItemSubtype::ReadSubtype(int itemSubTypeId)
	{
		AutoCreateTable();
		auto records = Common::Execute(QUERY_SUBTYPE, itemSubTypeId);
		if (!records.empty())
		{
			return common::Data::ToOptionalInt(records.front()[FIELD_SUBTYPE]);
		}
		return std::nullopt;
	}
	
	void ItemSubtype::Clear()
	{
		AutoCreateTable();
		Common::Execute(DELETE_ALL);
	}
}
