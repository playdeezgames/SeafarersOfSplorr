#include <Common.Data.h>
#include <Common.Utility.h>
#include "Data.Game.Common.h"
#include "Data.Game.World.h"
#include <string>
namespace data::game//20211010
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [Worlds]([WorldId] INT NOT NULL UNIQUE,[Version] INT NOT NULL,[Width] REAL NOT NULL,[Height] REAL NOT NULL,[MinimumIslandDistance] REAL NOT NULL,[ViewDistance] REAL NOT NULL,[DockDistance] REAL NOT NULL,[WindHeading] REAL NOT NULL);";
	static const auto AutoCreateWorldsTable = data::game::Common::Run(CREATE_TABLE);
	
	static const std::string QUERY_ITEM = "SELECT [Version],[Width],[Height],[MinimumIslandDistance],[ViewDistance],[DockDistance],[WindHeading] FROM [Worlds] WHERE [WorldId] = {};";
	static const std::string REPLACE_ITEM = "REPLACE INTO [Worlds]([WorldId],[Version],[Width],[Height],[MinimumIslandDistance],[ViewDistance],[DockDistance],[WindHeading]) VALUES ({},{},{},{},{},{},{},{});";

	static const std::string FIELD_VERSION = "Version";
	static const std::string FIELD_WIDTH = "Width";
	static const std::string FIELD_HEIGHT = "Height";
	static const std::string FIELD_MINIMUM_ISLAND_DISTANCE = "MinimumIslandDistance";
	static const std::string FIELD_VIEW_DISTANCE = "ViewDistance";
	static const std::string FIELD_DOCK_DISTANCE = "DockDistance";
	static const std::string FIELD_WIND_HEADING = "WindHeading";

	static const int WORLD_ID = 1;

	void World::Write(const World& data)
	{
		AutoCreateWorldsTable();
		data::game::Common::Execute(
			REPLACE_ITEM, 
			WORLD_ID,
			data.version,
			data.size.GetX(),
			data.size.GetY(),
			data.minimumIslandDistance,
			data.viewDistance,
			data.dockDistance,
			data.windHeading);
	}

	static World ToWorld(const std::map<std::string, std::string> record)
	{
		return 
		{
			common::Data::ToInt(record.find(FIELD_VERSION)->second),
			{
				common::Data::ToDouble(record.find(FIELD_WIDTH)->second),
				common::Data::ToDouble(record.find(FIELD_HEIGHT)->second)
			},
			common::Data::ToDouble(record.find(FIELD_MINIMUM_ISLAND_DISTANCE)->second),
			common::Data::ToDouble(record.find(FIELD_VIEW_DISTANCE)->second),
			common::Data::ToDouble(record.find(FIELD_DOCK_DISTANCE)->second),
			common::Data::ToDouble(record.find(FIELD_WIND_HEADING)->second)
		};
	}

	std::optional<World> World::Read()
	{
		AutoCreateWorldsTable();
		auto result = data::game::Common::Execute(QUERY_ITEM, WORLD_ID);
		if (!result.empty())
		{
			return ToWorld(result.front());
		}
		return std::nullopt;
	}
}