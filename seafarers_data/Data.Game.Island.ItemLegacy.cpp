#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Island.ItemLegacy.h"
namespace data::game::island
{
	using namespace std::string_literals;
	static const std::string CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [IslandItems]
		(
			[IslandId] INT NOT NULL,
			[ItemId] INT NOT NULL, 
			UNIQUE([IslandId],[ItemId])
		);)"s;
	static const std::string REPLACE_ITEM = 
		R"(REPLACE INTO [IslandItems]
		(
			[IslandId],
			[ItemId]
		) 
		VALUES({},{});)"s;
	static const std::string DELETE_ITEM = 
		R"(DELETE FROM[IslandItems]
		WHERE 
			[IslandId]={} 
			AND [ItemId]={};)"s;
	static const std::string DELETE_ALL = 
		R"(DELETE FROM [IslandItems];)"s;
	static const std::string QUERY_ITEM = 
		R"(SELECT 
			[ItemId] 
		FROM [IslandItems] 
		WHERE 
			[IslandId]={} 
			AND [ItemId]={};)"s;
	static const std::string QUERY_ALL = 
		R"(SELECT 
			[ItemId] 
		FROM [IslandItems] 
		WHERE 
			[IslandId]={};)"s;

	static const std::string FIELD_ISLAND_ID = "IslandId";
	static const std::string FIELD_ITEM_ID = "ItemId";

	static const auto AutoCreateIslandItemTable = data::game::Common::Run(CREATE_TABLE);

	void ItemLegacy::Write(int islandId, int item)
	{
		AutoCreateIslandItemTable();
		data::game::Common::Execute(
			REPLACE_ITEM, 
			islandId, 
			item);
	}

	void ItemLegacy::Clear(int islandId, int item)
	{
		AutoCreateIslandItemTable();
		data::game::Common::Execute(
			DELETE_ITEM, 
			islandId, 
			item);
	}

	bool ItemLegacy::Read(int islandId, int item)
	{
		AutoCreateIslandItemTable();
		return !data::game::Common::Execute(QUERY_ITEM, islandId, item).empty();
	}

	void ItemLegacy::ClearAll()
	{
		AutoCreateIslandItemTable();
		data::game::Common::Execute(DELETE_ALL);
	}

	std::set<int> ItemLegacy::GetAll(int islandId)
	{
		std::set<int> result;
		AutoCreateIslandItemTable();
		auto records = data::game::Common::Execute(QUERY_ALL, islandId);
		for (auto& record : records)
		{
			result.insert(common::Data::ToInt(record[FIELD_ITEM_ID]));
		}
		return result;
	}
}
