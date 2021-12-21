#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Island.Market.h"
namespace data::game::island
{
	static const std::string CREATE_TABLE = 
		"CREATE TABLE IF NOT EXISTS [IslandMarkets]"
		"("
			"[IslandId] INT NOT NULL, "
			"[CommodityId] INT NOT NULL,"
			"[Supply] REAL NOT NULL, "
			"[Demand] REAL NOT NULL, "
			"[Purchases] REAL NOT NULL, "
			"[Sales] REAL NOT NULL, "
			"UNIQUE([IslandId],[CommodityId])"
		");";
	static const std::string REPLACE_ITEM = 
		"REPLACE INTO [IslandMarkets]"
		"("
			"[IslandId],"
			"[CommodityId],"
			"[Supply],"
			"[Demand],"
			"[Purchases],"
			"[Sales]"
		") "
		"VALUES ({},{},{},{},{},{})";
	static const std::string QUERY_ITEM = 
		"SELECT "
			"[Supply], "
			"[Demand], "
			"[Purchases], "
			"[Sales] "
		"FROM [IslandMarkets] "
		"WHERE "
			"[IslandId]={} "
			"AND [CommodityId]={};";
	static const std::string QUERY_ALL = 
		"SELECT "
			"[CommodityId], "
			"[Supply], "
			"[Demand], "
			"[Purchases], "
			"[Sales] "
		"FROM [IslandMarkets] "
		"WHERE "
			"[IslandId]={};";
	static const std::string DELETE_ALL = 
		"DELETE FROM [IslandMarkets];";

	static const std::string FIELD_ISLAND_ID = "IslandId";
	static const std::string FIELD_COMMODITY_ID = "CommodityId";
	static const std::string FIELD_SUPPLY = "Supply";
	static const std::string FIELD_DEMAND = "Demand";
	static const std::string FIELD_PURCHASES = "Purchases";
	static const std::string FIELD_SALES = "Sales";

	static const auto AutoCreateIslandMarketsTable = data::game::Common::Run(CREATE_TABLE);

	void Market::Write(int islandId, int commodityId, const Market& data)
	{
		AutoCreateIslandMarketsTable();
		data::game::Common::Execute(
			REPLACE_ITEM, 
			islandId, 
			commodityId, 
			data.supply, 
			data.demand, 
			data.purchases, 
			data.sales
		);
	}

	static Market ToMarket(const std::map<std::string, std::string> record)
	{
		return
		{
			common::Data::ToDouble(record.find(FIELD_SUPPLY)->second),
			common::Data::ToDouble(record.find(FIELD_DEMAND)->second),
			common::Data::ToDouble(record.find(FIELD_PURCHASES)->second),
			common::Data::ToDouble(record.find(FIELD_SALES)->second)
		};
	}

	std::optional<Market> Market::Read(int islandId, int commodityId)
	{
		AutoCreateIslandMarketsTable();
		auto records =
			data::game::Common::Execute(
				QUERY_ITEM, 
				islandId, 
				commodityId
			);
		if (!records.empty())
		{
			return ToMarket(records.front());
		}
		return std::nullopt;
	}

	std::map<int, Market> Market::All(int islandId)
	{
		AutoCreateIslandMarketsTable();
		auto records =
			data::game::Common::Execute(
				QUERY_ALL, 
				islandId);
		std::map<int, Market> result;
		for (auto& record : records)
		{
			result[common::Data::ToInt(record[FIELD_COMMODITY_ID])] =
				ToMarket(record);
		}
		return result;
	}

	void Market::Clear()
	{
		AutoCreateIslandMarketsTable();
		data::game::Common::Execute(DELETE_ALL);
	}
}