#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.ItemSubtype.Commodity.h"
namespace data::game::item_subtype
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [ItemSubtypeCommodities]([ItemSubtypeId] INT NOT NULL,[CommodityId] INT NOT NULL,[Amount] REAL NOT NULL,UNIQUE([ItemSubtypeId],[CommodityId]));";
	static const std::string REPLACE_ITEM = "REPLACE INTO [ItemSubtypeCommodities]([ItemSubtypeId],[CommodityId],[Amount]) VALUES({},{},{});";
	static const std::string QUERY_AMOUNT = "SELECT [Amount] FROM [ItemSubtypeCommodities] WHERE [ItemSubtypeId]={} AND [CommodityId]={};";
	static const std::string QUERY_COMMODITY_ID = "SELECT [CommodityId] FROM [ItemSubtypeCommodities] WHERE [ItemSubtypeId]={};";
	static const std::string DELETE_ALL = "DELETE FROM [ItemSubtypeCommodities];";

	static const std::string FIELD_AMOUNT = "Amount";
	static const std::string FIELD_COMMODITY_ID = "CommodityId";

	static const auto AutoCreateTable = Common::Run(CREATE_TABLE);

	void Commodity::Write(int itemSubtypeId, int commodityId, double amount)
	{
		AutoCreateTable();
		Common::Execute(REPLACE_ITEM, itemSubtypeId, commodityId, amount);
	}

	std::optional<double> Commodity::Read(int itemSubtypeId, int commodityId)
	{
		AutoCreateTable();
		auto records = Common::Execute(QUERY_AMOUNT, itemSubtypeId, commodityId);
		if (!records.empty())
		{
			return common::Data::ToOptionalInt(records.front()[FIELD_AMOUNT]);
		}
		return std::nullopt;
	}

	std::list<int> Commodity::All(int itemSubtypeId)
	{
		AutoCreateTable();
		std::list<int> result;
		auto records = Common::Execute(QUERY_COMMODITY_ID, itemSubtypeId);
		for (auto record : records)
		{
			result.push_back(common::Data::ToInt(record[FIELD_COMMODITY_ID]));
		}
		return result;
	}

	void Commodity::Clear()
	{
		AutoCreateTable();
		Common::Execute(DELETE_ALL);
	}
}