#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.DemigodItem.h"
namespace data::game
{
	static const std::string CREATE_TABLE = 
		"CREATE TABLE IF NOT EXISTS [DemigodItems]"
		"("
			"[DemigodId] INT NOT NULL,"
			"[ItemId] INT NOT NULL,"
			"[Favor] REAL NOT NULL,"
			"UNIQUE([DemigodId],[ItemId])"
		");";
	static const std::string DELETE_ALL = 
		"DELETE FROM [DemigodItems];";
	static const std::string QUERY_ITEM = 
		"SELECT "
			"[Favor] "
		"FROM [DemigodItems] "
		"WHERE "
			"[DemigodId]={} "
			"AND [ItemId]={};";
	static const std::string REPLACE_ITEM = 
		"REPLACE INTO [DemigodItems]"
		"("
			"[DemigodId],"
			"[ItemId],"
			"[Favor]"
		") "
		"VALUES({},{},{});";

	static const std::string FIELD_FAVOR = "Favor";

	static const auto AutoCreateDemigodItemsTable = Common::Run(CREATE_TABLE);

	std::optional<double> DemigodItem::Read(int demigodId, int itemId)
	{
		AutoCreateDemigodItemsTable();
		auto records = 
			Common::Execute(
				QUERY_ITEM, 
				demigodId,
				itemId);
		if (!records.empty())
		{
			return common::Data::ToOptionalDouble(records.front()[FIELD_FAVOR]);
		}
		return std::nullopt;
	}

	void DemigodItem::Write(int demigodId, int itemId, double favor)
	{
		AutoCreateDemigodItemsTable();
		Common::Execute(
			REPLACE_ITEM, 
			demigodId, 
			itemId, 
			favor);
	}

	void DemigodItem::Clear()
	{
		AutoCreateDemigodItemsTable();
		Common::Execute(DELETE_ALL);
	}
}
