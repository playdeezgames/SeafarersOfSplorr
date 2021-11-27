#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Island.Known.h"
#include <string>
namespace data::game::island
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [KnownIslands]([IslandId] INT NOT NULL, UNIQUE([IslandId]));";
	static const std::string QUERY_ITEM = "SELECT [IslandId] FROM [KnownIslands] WHERE [IslandId]={};";
	static const std::string QUERY_ALL = "SELECT [IslandId] FROM [KnownIslands];";
	static const std::string REPLACE_ITEM = "REPLACE INTO [KnownIslands]([IslandId]) VALUES({});";
	static const std::string DELETE_ALL = "DELETE FROM [KnownIslands];";

	static const std::string FIELD_ISLAND_ID = "IslandId";

	static const auto AutoCreateKnownIslandsTable = data::game::Common::Run(CREATE_TABLE);

	void Known::Write(int islandId)
	{
		AutoCreateKnownIslandsTable();
		data::game::Common::Execute(REPLACE_ITEM, islandId);
	}

	bool Known::Read(int islandId)
	{
		AutoCreateKnownIslandsTable();
		return !data::game::Common::Execute(QUERY_ITEM, islandId).empty();
	}

	void Known::Clear()
	{
		AutoCreateKnownIslandsTable();
		data::game::Common::Execute(DELETE_ALL);
	}

	std::list<int> Known::All()
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