#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Island.h"
#include "Data.Game.Ship.h"
#include "Data.Game.Ship.CurrentIsland.h"
namespace data::game::ship
{
	using namespace std::string_literals;
	static const std::string CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [ShipCurrentIslands]
		(
			[ShipId] INT NOT NULL, 
			[IslandId] INT NOT NULL, 
			UNIQUE([ShipId]),
			FOREIGN KEY ([ShipId]) REFERENCES [Ships]([ShipId]),
			FOREIGN KEY ([IslandId]) REFERENCES [Islands]([IslandId])
		);)"s;
	static const std::string REPLACE_ITEM = 
		R"(REPLACE INTO [ShipCurrentIslands]
		(
			[ShipId],
			[IslandId]
		) 
		VALUES({},{});)"s;
	static const std::string QUERY_ITEM = 
		R"(SELECT 
			[IslandId] 
		FROM [ShipCurrentIslands] 
		WHERE 
			[ShipId]={};)"s;
	static const std::string DELETE_ITEM = 
		R"(DELETE FROM[ShipCurrentIslands] 
		WHERE 
			[ShipId]={};)"s;
	static const std::string DELETE_ALL = 
		"DELETE FROM [ShipCurrentIslands];";

	static const std::string FIELD_ISLAND_ID = "IslandId";

	void CurrentIsland::Initialize()
	{
		Island::Initialize();
		Ship::Initialize();
		Common::Execute(CREATE_TABLE);
	}

	void CurrentIsland::Write(int shipId, int islandId)
	{
		Initialize();
		Common::Execute(REPLACE_ITEM, shipId, islandId);
	}

	std::optional<int> CurrentIsland::Read(int shipId)
	{
		Initialize();
		auto records = Common::Execute(QUERY_ITEM, shipId);
		if (!records.empty())
		{
			return common::Data::ToInt(records.front()[FIELD_ISLAND_ID]);
		}
		return std::nullopt;
	}

	void CurrentIsland::Clear(int shipId)
	{
		Initialize();
		Common::Execute(DELETE_ITEM, shipId);
	}

	void CurrentIsland::ClearAll()
	{
		Initialize();
		Common::Execute(DELETE_ALL);
	}
}