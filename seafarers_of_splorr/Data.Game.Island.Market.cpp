#include "Common.Data.h"
#include "Data.Game.Common.h"
#include "Data.Game.Island.Market.h"
#include <format>
namespace data::game::island::Market
{
	const std::string FIELD_X = "X";
	const std::string FIELD_Y = "Y";
	const std::string FIELD_COMMODITY_ID = "CommodityId";
	const std::string FIELD_SUPPLY = "Supply";
	const std::string FIELD_DEMAND = "Demand";
	const std::string FIELD_PURCHASES = "Purchases";
	const std::string FIELD_SALES = "Sales";
	const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [IslandMarkets]([X] REAL NOT NULL, [Y] REAL NOT NULL, [CommodityId] INT NOT NULL,[Supply] REAL NOT NULL, [Demand] REAL NOT NULL, [Purchases] REAL NOT NULL, [Sales] REAL NOT NULL, UNIQUE([X],[Y],[CommodityId]));";
	const std::string REPLACE_ITEM = "REPLACE INTO [IslandMarkets]([X],[Y],[CommodityId],[Supply],[Demand],[Purchases],[Sales]) VALUES ({:.4f},{:.4f},{},{},{},{},{})";
	const std::string QUERY_ITEM = "SELECT [Supply], [Demand], [Purchases], [Sales] FROM [IslandMarkets] WHERE [X]={:.4f} AND [Y]={:.4f} AND [CommodityId]={};";
	const std::string QUERY_ALL = "SELECT [CommodityId], [Supply], [Demand], [Purchases], [Sales] FROM [IslandMarkets] WHERE [X]={:.4f} AND [Y]={:.4f};";
	const std::string DELETE_ALL = "DELETE FROM [IslandMarkets];";

	static void AutoCreateIslandMarketsTable()
	{
		data::game::Common::Execute(CREATE_TABLE);
	}

	void Write(const common::XY<double>& location, int commodityId, const MarketData& data)
	{
		AutoCreateIslandMarketsTable();
		data::game::Common::Execute(
			std::format(REPLACE_ITEM, location.GetX(), location.GetY(), commodityId, data.supply, data.demand, data.purchases, data.sales)
		);
	}

	std::optional<MarketData> Read(const common::XY<double>& location, int commodityId)
	{
		AutoCreateIslandMarketsTable();
		auto records =
			data::game::Common::Execute(
				std::format(QUERY_ITEM, location.GetX(), location.GetY(), commodityId)
			);
		if (!records.empty())
		{
			auto& record = records.front();
			return std::optional<MarketData>(
				{
					common::Data::StringToDouble(record[FIELD_SUPPLY]),
					common::Data::StringToDouble(record[FIELD_DEMAND]),
					common::Data::StringToDouble(record[FIELD_PURCHASES]),
					common::Data::StringToDouble(record[FIELD_SALES])
				});
		}
		return std::nullopt;
	}

	std::map<int, MarketData> All(const common::XY<double>& location)
	{
		AutoCreateIslandMarketsTable();
		auto records =
			data::game::Common::Execute(
				std::format(QUERY_ALL, location.GetX(), location.GetY())
			);
		std::map<int, MarketData> result;
		for (auto& record : records)
		{
			result[common::Data::StringToInt(record[FIELD_COMMODITY_ID])] =
				{
					common::Data::StringToDouble(record[FIELD_SUPPLY]),
					common::Data::StringToDouble(record[FIELD_DEMAND]),
					common::Data::StringToDouble(record[FIELD_PURCHASES]),
					common::Data::StringToDouble(record[FIELD_SALES])
				};
		}
		return result;
	}

	void Clear()
	{
		AutoCreateIslandMarketsTable();
		data::game::Common::Execute(DELETE_ALL);
	}
}