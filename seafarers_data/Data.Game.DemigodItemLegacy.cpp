#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.DemigodItemLegacy.h"
namespace data::game
{
	using namespace std::string_literals;
	static const std::string CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [DemigodItems]
		(
			[DemigodId] INT NOT NULL,
			[ItemId] INT NOT NULL,
			[Favor] REAL NOT NULL,
			UNIQUE([DemigodId],[ItemId])
		);)"s;
	static const std::string DELETE_ALL = 
		R"(DELETE FROM [DemigodItems];)"s;
	static const std::string QUERY_ITEM = 
		R"(SELECT 
			[Favor] 
		FROM [DemigodItems] 
		WHERE 
			[DemigodId]={} 
			AND [ItemId]={};)"s;
	static const std::string REPLACE_ITEM = 
		R"(REPLACE INTO [DemigodItems]
		(
			[DemigodId],
			[ItemId],
			[Favor]
		) 
		VALUES({},{},{});)"s;

	static const std::string FIELD_FAVOR = "Favor";

	static const auto AutoCreateDemigodItemsTable = Common::Run(CREATE_TABLE);

	std::optional<double> DemigodItemLegacy::Read(int demigodId, int itemId)
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

	void DemigodItemLegacy::Write(int demigodId, int itemId, double favor)
	{
		AutoCreateDemigodItemsTable();
		Common::Execute(
			REPLACE_ITEM, 
			demigodId, 
			itemId, 
			favor);
	}

	void DemigodItemLegacy::Clear()
	{
		AutoCreateDemigodItemsTable();
		Common::Execute(DELETE_ALL);
	}
}
