#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Island.h"
#include "Data.Game.Island.Market.h"
namespace data::game::island
{
	using namespace std::string_literals;
	static const std::string CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [IslandMarkets]
		(
			[IslandId] INT NOT NULL, 
			[Commodity] INT NOT NULL,
			[Supply] REAL NOT NULL, 
			[Demand] REAL NOT NULL, 
			[Purchases] REAL NOT NULL, 
			[Sales] REAL NOT NULL, 
			UNIQUE([IslandId],[Commodity]),
			FOREIGN KEY ([IslandId]) REFERENCES [Islands]([IslandId])
		);)"s;
	static const std::string REPLACE_ITEM = 
		R"(REPLACE INTO [IslandMarkets]
		(
			[IslandId],
			[Commodity],
			[Supply],
			[Demand],
			[Purchases],
			[Sales]
		) 
		VALUES ({},{},{},{},{},{});)"s;
	static const std::string QUERY_ITEM = 
		R"(SELECT 
			[Supply], 
			[Demand], 
			[Purchases], 
			[Sales] 
		FROM [IslandMarkets] 
		WHERE 
			[IslandId]={} 
			AND [Commodity]={};)"s;
	static const std::string QUERY_ALL = 
		R"(SELECT 
			[Commodity], 
			[Supply], 
			[Demand], 
			[Purchases], 
			[Sales] 
		FROM [IslandMarkets] 
		WHERE 
			[IslandId]={};)"s;
	static const std::string DELETE_ALL = 
		R"(DELETE FROM [IslandMarkets];)"s;

	static const std::string FIELD_ISLAND_ID = "IslandId";
	static const std::string FIELD_COMMODITY = "Commodity";
	static const std::string FIELD_SUPPLY = "Supply";
	static const std::string FIELD_DEMAND = "Demand";
	static const std::string FIELD_PURCHASES = "Purchases";
	static const std::string FIELD_SALES = "Sales";

	void Market::Initialize()
	{
		Island::Initialize();
		Common::Execute(CREATE_TABLE);
	}


	void Market::Write(int islandId, int commodityId, const Data& data)
	{
		Initialize();
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

	static Market::Data ToMarket(const std::map<std::string, std::string> record)
	{
		return
		{
			common::Data::ToDouble(record.find(FIELD_SUPPLY)->second),
			common::Data::ToDouble(record.find(FIELD_DEMAND)->second),
			common::Data::ToDouble(record.find(FIELD_PURCHASES)->second),
			common::Data::ToDouble(record.find(FIELD_SALES)->second)
		};
	}

	std::optional<Market::Data> Market::Read(int islandId, int commodityId)
	{
		Initialize();
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

	std::map<int, Market::Data> Market::All(int islandId)
	{
		Initialize();
		auto records =
			data::game::Common::Execute(
				QUERY_ALL, 
				islandId);
		std::map<int, Market::Data> result;
		for (auto& record : records)
		{
			result[common::Data::ToInt(record[FIELD_COMMODITY])] =
				ToMarket(record);
		}
		return result;
	}

	void Market::Clear()
	{
		Initialize();
		data::game::Common::Execute(DELETE_ALL);
	}
}