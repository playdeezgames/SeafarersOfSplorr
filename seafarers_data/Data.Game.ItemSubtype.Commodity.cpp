#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.ItemSubtype.Commodity.h"
namespace data::game::item_subtype
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [ItemSubtypeCommodities]([ItemSubtypeId] INT NOT NULL,[CommodityId] INT NOT NULL,[Amount] REAL NOT NULL,UNIQUE([ItemSubtypeId],[CommodityId]));";
	static const std::string REPLACE_ITEM = "REPLACE INTO [ItemSubtypeCommodities]([ItemSubtypeId],[CommodityId],[Amount]) VALUES({},{},{});";
	static const std::string DELETE_ALL = "DELETE FROM [ItemSubtypeCommodities];";

	static const auto AutoCreateTable = Common::Run(CREATE_TABLE);

	void Commodity::Write(int itemSubtypeId, int commodityId, double amount)
	{
		AutoCreateTable();
		Common::Execute(REPLACE_ITEM, itemSubtypeId, commodityId, amount);
	}

	void Commodity::Clear()
	{
		AutoCreateTable();
		Common::Execute(DELETE_ALL);
	}
}