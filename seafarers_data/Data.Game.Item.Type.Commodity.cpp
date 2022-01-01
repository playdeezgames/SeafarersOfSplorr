#include <algorithm>
#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Item.Type.h"
#include "Data.Game.Item.Type.Commodity.h"
#include <iterator>
namespace data::game::item::type
{
	using namespace std::string_literals;
	static const std::string CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [ItemTypeCommodities]
		(
			[ItemTypeId] INT NOT NULL,
			[Commodity] INT NOT NULL,
			[Amount] REAL NOT NULL,
			UNIQUE([ItemTypeId],[Commodity]),
			FOREIGN KEY ([ItemTypeId]) REFERENCES [ItemTypes]([ItemTypeId])
		);)"s;
	static const std::string REPLACE_ITEM = 
		R"(REPLACE INTO [ItemTypeCommodities]
		(
			[ItemTypeId],
			[Commodity],
			[Amount]
		) 
		VALUES({},{},{});)"s;
	static const std::string QUERY_AMOUNT = 
		R"(SELECT 
			[Amount] 
		FROM [ItemTypeCommodities] 
		WHERE 
			[ItemTypeId]={} 
			AND [Commodity]={};)"s;
	static const std::string QUERY_COMMODITY = 
		R"(SELECT 
			[Commodity] 
		FROM [ItemTypeCommodities] 
		WHERE 
			[ItemTypeId]={};)"s;
	static const std::string DELETE_ALL = 
		R"(DELETE FROM [ItemTypeCommodities];)"s;

	static const std::string FIELD_AMOUNT = "Amount";
	static const std::string FIELD_COMMODITY = "Commodity";

	void Commodity::Initialize()
	{
		Type::Initialize();
		Common::Execute(CREATE_TABLE);
	}

	void Commodity::Write(int itemTypeId, int commodity, double amount)
	{
		Initialize();
		Common::Execute(REPLACE_ITEM, itemTypeId, commodity, amount);
	}

	std::optional<double> Commodity::ReadAmount(int itemTypeId, int commodity)
	{
		Initialize();
		return Common::TryToDouble(
			Common::TryExecuteForOne(QUERY_AMOUNT, itemTypeId, commodity), 
			FIELD_AMOUNT);
	}

	std::list<int> Commodity::AllCommodities(int itemTypeId)
	{
		Initialize();
		std::list<int> result;
		auto records = Common::Execute(QUERY_COMMODITY, itemTypeId);
		std::transform(
			records.begin(),
			records.end(),
			std::back_inserter(result),
			Common::DoToInt(FIELD_COMMODITY));
		return result;
	}

	void Commodity::Clear()
	{
		Initialize();
		Common::Execute(DELETE_ALL);
	}
}