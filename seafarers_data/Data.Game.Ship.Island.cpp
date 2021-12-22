#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Island.h"
#include "Data.Game.Ship.h"
#include "Data.Game.Ship.Island.h"
namespace data::game::ship
{
	using namespace std::string_literals;
	static const std::string CREATE_TABLE =
		R"(CREATE TABLE IF NOT EXISTS [ShipIslands]
		(
			[ShipId] INT NOT NULL,
			[IslandId] INT NOT NULL,
			UNIQUE([ShipId],[IslandId]),
			FOREIGN KEY ([ShipId]) REFERENCES [Ships]([ShipId]),
			FOREIGN KEY ([IslandId]) REFERENCES [Islands]([IslandId])
		);)"s;
	static const std::string REPLACE_ITEM =
		R"(REPLACE INTO [ShipIslands]
		(
			[ShipId],
			[IslandId]
		) 
		VALUES({},{});)"s;
	static const std::string DELETE_ITEM =
		R"(DELETE FROM [ShipIslands] 
		WHERE 
			[ShipId]={};)"s;
	static const std::string DELETE_ALL =
		R"(DELETE FROM [ShipIslands];)"s;
	static const std::string QUERY_ITEM =
		R"(SELECT 
			[IslandId] 
		FROM [ShipIslands] 
		WHERE 
			[ShipId]={} 
			AND [IslandId]={};)"s;

	void Island::Initialize()
	{
		Island::Initialize();
		Ship::Initialize();
		Common::Execute(CREATE_TABLE);
	}

	void Island::Write(int shipId, int islandId)
	{
		Initialize();
		Common::Execute(REPLACE_ITEM, shipId, islandId);
	}

	bool Island::Read(int shipId, int islandId)
	{
		Initialize();
		return !Common::Execute(QUERY_ITEM, shipId, islandId).empty();
	}

	void Island::Clear(int shipId)
	{
		Initialize();
		Common::Execute(DELETE_ITEM, shipId);
	}

	void Island::Clear()
	{
		Initialize();
		Common::Execute(DELETE_ALL);
	}
}