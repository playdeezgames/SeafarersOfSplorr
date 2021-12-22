#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.World.h"
namespace data::game
{
	static const std::string CREATE_TABLE = 
		"CREATE TABLE IF NOT EXISTS [Worlds]"
		"("
			"[WorldId] INT NOT NULL UNIQUE,"
			"[Version] INT NOT NULL,"
			"[Width] REAL NOT NULL,"
			"[Height] REAL NOT NULL,"
			"[MinimumIslandDistance] REAL NOT NULL,"
			"[ViewDistance] REAL NOT NULL,"
			"[DockDistance] REAL NOT NULL,"
			"[WindHeading] REAL NOT NULL,"
			"[CurrencyItemSubtypeId] INT NOT NULL,"
			"[UnfoulingLaborMultiplier] REAL NOT NULL,"
			"[ReputationReward] REAL NOT NULL,"
			"[ReputationPenalty] REAL NOT NULL"
		");";
	static const std::string QUERY_ITEM = 
		"SELECT "
			"[Version],"
			"[Width],"
			"[Height],"
			"[MinimumIslandDistance],"
			"[ViewDistance],"
			"[DockDistance],"
			"[WindHeading],"
			"[UnfoulingLaborMultiplier],"
			"[ReputationReward],"
			"[ReputationPenalty],"
			"[CurrencyItemSubtypeId] "
		"FROM [Worlds] "
		"WHERE "
			"[WorldId] = {};";
	static const std::string REPLACE_ITEM = 
		"REPLACE INTO [Worlds]"
		"("
			"[WorldId],"
			"[Version],"
			"[Width],"
			"[Height],"
			"[MinimumIslandDistance],"
			"[ViewDistance],"
			"[DockDistance],"
			"[WindHeading],"
			"[CurrencyItemSubtypeId],"
			"[UnfoulingLaborMultiplier],"
			"[ReputationReward],"
			"[ReputationPenalty]"
		") "
		"VALUES ({},{},{},{},{},{},{},{},{},{},{},{});";

	static const std::string FIELD_VERSION = "Version";
	static const std::string FIELD_WIDTH = "Width";
	static const std::string FIELD_HEIGHT = "Height";
	static const std::string FIELD_MINIMUM_ISLAND_DISTANCE = "MinimumIslandDistance";
	static const std::string FIELD_VIEW_DISTANCE = "ViewDistance";
	static const std::string FIELD_DOCK_DISTANCE = "DockDistance";
	static const std::string FIELD_WIND_HEADING = "WindHeading";
	static const std::string FIELD_CURRENCY_ITEM_SUBTYPE_ID = "CurrencyItemSubtypeId";

	static const std::string FIELD_UNFOULING_LABOR_MULTIPLIER = "UnfoulingLaborMultiplier";
	static const std::string FIELD_REPUTATION_REWARD = "ReputationReward";
	static const std::string FIELD_REPUTATION_PENALTY = "ReputationPenalty";

	static const int WORLD_ID = 1;

	void World::Initialize()
	{
		Common::Execute(CREATE_TABLE);
	}


	void World::Write(const World& data)
	{
		Initialize();
		data::game::Common::Execute(
			REPLACE_ITEM, 
			WORLD_ID,
			data.version,
			data.size.GetX(),
			data.size.GetY(),
			data.minimumIslandDistance,
			data.viewDistance,
			data.dockDistance,
			data.windHeading,
			data.currencyItemTypeId,
			data.unfoulingLaborMultiplier,
			data.reputationReward,
			data.reputationPenalty);
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
			common::Data::ToDouble(record.find(FIELD_UNFOULING_LABOR_MULTIPLIER)->second),
			common::Data::ToDouble(record.find(FIELD_REPUTATION_REWARD)->second),
			common::Data::ToDouble(record.find(FIELD_REPUTATION_PENALTY)->second)
		};
	}

	std::optional<World> World::Read()
	{
		Initialize();
		auto result = data::game::Common::Execute(QUERY_ITEM, WORLD_ID);
		if (!result.empty())
		{
			return ToWorld(result.front());
		}
		return std::nullopt;
	}
}