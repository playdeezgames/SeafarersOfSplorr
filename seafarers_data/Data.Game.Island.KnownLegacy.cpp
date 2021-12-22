#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Island.KnownLegacy.h"
namespace data::game::island
{
	static const std::string CREATE_TABLE = 
		"CREATE TABLE IF NOT EXISTS [KnownIslandsLegacy]"
		"("
			"[IslandId] INT NOT NULL, "
			"UNIQUE([IslandId])"
		");";
	static const std::string QUERY_ITEM = 
		"SELECT "
			"[IslandId] "
		"FROM [KnownIslandsLegacy] "
		"WHERE "
			"[IslandId]={};";
	static const std::string QUERY_ALL = 
		"SELECT "
			"[IslandId] "
		"FROM [KnownIslandsLegacy];";
	static const std::string REPLACE_ITEM = 
		"REPLACE INTO [KnownIslandsLegacy]"
		"("
			"[IslandId]"
		") "
		"VALUES({});";
	static const std::string DELETE_ALL = 
		"DELETE FROM [KnownIslandsLegacy];";

	static const std::string FIELD_ISLAND_ID = "IslandId";

	static const auto AutoCreateKnownIslandsTable = data::game::Common::Run(CREATE_TABLE);

	void KnownLegacy::Write(int islandId)
	{
		AutoCreateKnownIslandsTable();
		data::game::Common::Execute(REPLACE_ITEM, islandId);
	}

	bool KnownLegacy::Read(int islandId)
	{
		AutoCreateKnownIslandsTable();
		return !data::game::Common::Execute(QUERY_ITEM, islandId).empty();
	}

	void KnownLegacy::Clear()
	{
		AutoCreateKnownIslandsTable();
		data::game::Common::Execute(DELETE_ALL);
	}

	std::list<int> KnownLegacy::All()
	{
		AutoCreateKnownIslandsTable();
		auto records = data::game::Common::Execute(QUERY_ALL);
		std::list<int> result;
		for (auto& record : records)
		{
			result.push_back(common::Data::ToInt(record[FIELD_ISLAND_ID]));
		}
		return result;
	}
}