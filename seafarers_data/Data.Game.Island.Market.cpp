#include <algorithm>
#include "Data.Game.Common.h"
#include "Data.Game.Island.h"
#include "Data.Game.Island.Market.h"
namespace data::game::island
{
	using namespace std::string_view_literals;
	static constexpr std::string_view CREATE_TABLE = 
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
		);)"sv;
	static constexpr std::string_view REPLACE_ITEM =
		R"(REPLACE INTO [IslandMarkets]
		(
			[IslandId],
			[Commodity],
			[Supply],
			[Demand],
			[Purchases],
			[Sales]
		) 
		VALUES ({},{},{},{},{},{});)"sv;
	static constexpr std::string_view QUERY_ITEM =
		R"(SELECT 
			[Supply], 
			[Demand], 
			[Purchases], 
			[Sales] 
		FROM [IslandMarkets] 
		WHERE 
			[IslandId]={} 
			AND [Commodity]={};)"sv;
	static constexpr std::string_view QUERY_ALL =
		R"(SELECT 
			[Commodity], 
			[Supply], 
			[Demand], 
			[Purchases], 
			[Sales] 
		FROM [IslandMarkets] 
		WHERE 
			[IslandId]={};)"sv;
	static constexpr std::string_view DELETE_ALL =
		R"(DELETE FROM [IslandMarkets];)"sv;

	static constexpr std::string_view FIELD_ISLAND_ID = "IslandId";
	static constexpr std::string_view FIELD_COMMODITY = "Commodity";
	static constexpr std::string_view FIELD_SUPPLY = "Supply";
	static constexpr std::string_view FIELD_DEMAND = "Demand";
	static constexpr std::string_view FIELD_PURCHASES = "Purchases";
	static constexpr std::string_view FIELD_SALES = "Sales";

	void Market::Initialize()
	{
		Island::Initialize();
		Common::Execute(CREATE_TABLE);
	}


	void Market::Write(int islandId, int commodity, const Data& marketData)
	{
		Initialize();
		data::game::Common::Execute(
			REPLACE_ITEM, 
			islandId, 
			commodity, 
			marketData.supply,
			marketData.demand,
			marketData.purchases,
			marketData.sales
		);
	}

	static Market::Data ToMarket(const Common::Record& record)
	{
		return
		{
			Common::ToDouble(record, FIELD_SUPPLY),
			Common::ToDouble(record, FIELD_DEMAND),
			Common::ToDouble(record, FIELD_PURCHASES),
			Common::ToDouble(record, FIELD_SALES)
		};
	}

	std::optional<Market::Data> Market::Read(int islandId, int commodity)
	{
		Initialize();
		auto record = Common::TryExecuteForOne(
				QUERY_ITEM, 
				islandId, 
				commodity
			);
		if (record)
		{
			return ToMarket(*record);
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
		std::transform(
			records.begin(),
			records.end(),
			std::inserter(result, result.end()),
			[](const Common::Record& record) 
			{
				return std::make_pair(
					Common::ToInt(record, FIELD_COMMODITY),
					ToMarket(record));
			});
		return result;
	}

	void Market::Clear()
	{
		Initialize();
		data::game::Common::Execute(DELETE_ALL);
	}
}