#include <algorithm>
#include "Data.Game.Common.h"
#include "Data.Game.Item.Type.h"
#include "Data.Game.Item.Type.Commodity.h"
namespace data::game::item::type
{
	using namespace std::string_view_literals;
	static constexpr std::string_view CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [ItemTypeCommodities]
		(
			[ItemTypeId] INT NOT NULL,
			[Commodity] INT NOT NULL,
			[Amount] REAL NOT NULL,
			UNIQUE([ItemTypeId],[Commodity]),
			FOREIGN KEY ([ItemTypeId]) REFERENCES [ItemTypes]([ItemTypeId])
		);)"sv;
	static constexpr std::string_view REPLACE_ITEM =
		R"(REPLACE INTO [ItemTypeCommodities]
		(
			[ItemTypeId],
			[Commodity],
			[Amount]
		) 
		VALUES({},{},{});)"sv;
	static constexpr std::string_view QUERY_AMOUNT =
		R"(SELECT 
			[Amount] 
		FROM [ItemTypeCommodities] 
		WHERE 
			[ItemTypeId]={} 
			AND [Commodity]={};)"sv;
	static constexpr std::string_view QUERY_COMMODITY =
		R"(SELECT 
			[Commodity] 
		FROM [ItemTypeCommodities] 
		WHERE 
			[ItemTypeId]={};)"sv;
	static constexpr std::string_view DELETE_ALL =
		R"(DELETE FROM [ItemTypeCommodities];)"sv;

	static constexpr std::string_view FIELD_AMOUNT = "Amount";
	static constexpr std::string_view FIELD_COMMODITY = "Commodity";

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

	std::vector<int> Commodity::AllCommodities(int itemTypeId)
	{
		Initialize();
		auto records = Common::Execute(QUERY_COMMODITY, itemTypeId);
		std::vector<int> result;
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