#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Item.Type.Commodity.h"
namespace data::game::item::type
{
	using namespace std::string_literals;
	static const std::string CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [ItemSubtypeCommodities]
		(
			[ItemSubtypeId] INT NOT NULL,
			[CommodityId] INT NOT NULL,
			[Amount] REAL NOT NULL,
			UNIQUE([ItemSubtypeId],[CommodityId])
		);)"s;
	static const std::string REPLACE_ITEM = 
		R"(REPLACE INTO [ItemSubtypeCommodities]
		(
			[ItemSubtypeId],
			[CommodityId],
			[Amount]
		) 
		VALUES({},{},{});)"s;
	static const std::string QUERY_AMOUNT = 
		R"(SELECT 
			[Amount] 
		FROM [ItemSubtypeCommodities] 
		WHERE 
			[ItemSubtypeId]={} 
			AND [CommodityId]={};)"s;
	static const std::string QUERY_COMMODITY_ID = 
		R"(SELECT 
			[CommodityId] 
		FROM [ItemSubtypeCommodities] 
		WHERE 
			[ItemSubtypeId]={};)"s;
	static const std::string DELETE_ALL = 
		R"(DELETE FROM [ItemSubtypeCommodities];)"s;

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
			return common::Data::ToOptionalDouble(records.front()[FIELD_AMOUNT]);
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