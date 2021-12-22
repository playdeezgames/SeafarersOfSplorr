#include <Common.Data.h>
#include <Common.RNG.h>
#include "Data.Game.Common.h"
#include "Data.Game.World.h"
namespace data::game
{
	using namespace std::string_literals;
	static const std::string CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [Worlds]
		(
			[WorldId] INT NOT NULL UNIQUE,
			[Version] INT NOT NULL,
			[Width] REAL NOT NULL,
			[Height] REAL NOT NULL,
			[MinimumIslandDistance] REAL NOT NULL,
			[ViewDistance] REAL NOT NULL,
			[DockDistance] REAL NOT NULL,
			[WindHeading] REAL NOT NULL,
			[CurrencyItemSubtypeId] INT NOT NULL,
			[Day] INT NOT NULL
		);)"s;
	static const std::string QUERY_ITEM = 
		R"(SELECT 
			[Version],
			[Width],
			[Height],
			[MinimumIslandDistance],
			[ViewDistance],
			[DockDistance],
			[WindHeading],
			[CurrencyItemSubtypeId],
			[Day]
		FROM [Worlds] 
		WHERE 
			[WorldId] = {};)"s;
	static const std::string REPLACE_ITEM = 
		R"(REPLACE INTO [Worlds]
		(
			[WorldId],
			[Version],
			[Width],
			[Height],
			[MinimumIslandDistance],
			[ViewDistance],
			[DockDistance],
			[WindHeading],
			[CurrencyItemSubtypeId],
			[Day]
		) 
		VALUES ({},{},{},{},{},{},{},{},{},{});)"s;

	static const std::string FIELD_VERSION = "Version";
	static const std::string FIELD_WIDTH = "Width";
	static const std::string FIELD_HEIGHT = "Height";
	static const std::string FIELD_MINIMUM_ISLAND_DISTANCE = "MinimumIslandDistance";
	static const std::string FIELD_VIEW_DISTANCE = "ViewDistance";
	static const std::string FIELD_DOCK_DISTANCE = "DockDistance";
	static const std::string FIELD_WIND_HEADING = "WindHeading";
	static const std::string FIELD_CURRENCY_ITEM_SUBTYPE_ID = "CurrencyItemSubtypeId";
	static const std::string FIELD_DAY = "Day";

	static const int EARLIEST_INITIAL_YEAR = 500;
	static const int LATEST_INITIAL_YEAR = 5000;
	static const int DAYS_PER_YEAR = 360;

	void World::Initialize()
	{
		Common::Execute(CREATE_TABLE);
	}


	void World::Write(int worldId, const World& data)
	{
		Initialize();
		data::game::Common::Execute(
			REPLACE_ITEM, 
			worldId,
			data.version,
			data.size.GetX(),
			data.size.GetY(),
			data.minimumIslandDistance,
			data.viewDistance,
			data.dockDistance,
			data.windHeading,
			data.currencyItemTypeId,
			common::RNG::FromRange(EARLIEST_INITIAL_YEAR, LATEST_INITIAL_YEAR) * DAYS_PER_YEAR + 
			common::RNG::FromRange(0, DAYS_PER_YEAR));
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
			common::Data::ToDouble(record.find(FIELD_WIND_HEADING)->second),
			common::Data::ToInt(record.find(FIELD_CURRENCY_ITEM_SUBTYPE_ID)->second),
			common::Data::ToInt(record.find(FIELD_DAY)->second)
		};
	}

	std::optional<World> World::Read(int worldId)
	{
		Initialize();
		auto result = data::game::Common::Execute(QUERY_ITEM, worldId);
		if (!result.empty())
		{
			return ToWorld(result.front());
		}
		return std::nullopt;
	}
}