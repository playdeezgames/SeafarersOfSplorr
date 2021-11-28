#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Ship.Docks.h"
namespace data::game::ship
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [ShipDocks]([ShipId] INT NOT NULL, [IslandId] INT NOT NULL, UNIQUE([ShipId],[IslandId]));";
	static const std::string REPLACE_ITEM = "REPLACE INTO [ShipDocks]([ShipId],[IslandId]) VALUES({},{});";
	static const std::string QUERY_ITEM = "SELECT [IslandId] FROM [ShipDocks] WHERE [ShipId]={};";
	static const std::string DELETE_ITEM = "DELETE FROM [ShipDocks] WHERE [ShipId]={};";
	static const std::string DELETE_ALL = "DELETE FROM [ShipDocks];";

	static const std::string FIELD_ISLAND_ID = "IslandId";

	static const auto AutoCreateTable = data::game::Common::Run(CREATE_TABLE);


	void Docks::Write(int shipId, int islandId)
	{
		AutoCreateTable();
		Common::Execute(REPLACE_ITEM, shipId, islandId);
	}

	std::optional<int> Docks::Read(int shipId)
	{
		AutoCreateTable();
		auto records = Common::Execute(QUERY_ITEM, shipId);
		if (!records.empty())
		{
			return common::Data::ToInt(records.front()[FIELD_ISLAND_ID]);
		}
		return std::nullopt;
	}

	void Docks::Clear(int shipId)
	{
		AutoCreateTable();
		Common::Execute(DELETE_ITEM, shipId);
	}

	void Docks::ClearAll()
	{
		AutoCreateTable();
		Common::Execute(DELETE_ALL);
	}
}