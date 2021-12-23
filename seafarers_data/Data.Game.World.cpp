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
	static const std::string QUERY_ITEM_SIZE =
		R"(SELECT 
			[Width],
			[Height] 
		FROM [Worlds] 
		WHERE 
			[WorldId]={};)";
	static const std::string QUERY_ITEM_COLUMN =
		R"(SELECT 
			[{}] 
		FROM [Worlds] 
		WHERE 
			[WorldId]={};)";
	static const std::string UPDATE_ITEM_COLUMN =
		R"(UPDATE [Worlds] 
		SET 
			[{}]={} 
		WHERE 
			[WorldId]={};)";
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

	void World::Initialize()
	{
		Common::Execute(CREATE_TABLE);
	}

	void World::Write(
		int worldId,
		int version,
		const common::XY<double>& size,
		double minimumIslandDistance,
		double viewDistance,
		double dockDistance,
		double windHeading,
		int currencyItemTypeId,
		int day)
	{
		Initialize();
		Common::Execute(
			REPLACE_ITEM,
			worldId,
			version,
			size.GetX(),
			size.GetY(),
			minimumIslandDistance,
			viewDistance,
			dockDistance,
			windHeading,
			currencyItemTypeId,
			day);
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

	std::optional<common::XY<double>> World::ReadSize(int worldId)
	{
		Initialize();
		auto record = Common::TryExecuteForOne(QUERY_ITEM_SIZE, worldId);
		if (record)
		{
			return common::XY(
				Common::ToDouble(record.value(), FIELD_WIDTH), 
				Common::ToDouble(record.value(), FIELD_HEIGHT));
		}
		return std::nullopt;
	}

	std::optional<double> World::ReadMinimumIslandDistance(int worldId)
	{
		Initialize();
		auto record = Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_MINIMUM_ISLAND_DISTANCE, worldId);
		if (record)
		{
			return Common::ToDouble(record.value(), FIELD_MINIMUM_ISLAND_DISTANCE);
		}
		return std::nullopt;
	}

	std::optional<double> World::ReadViewDistance(int worldId)
	{
		Initialize();
		auto record = Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_VIEW_DISTANCE, worldId);
		if (record)
		{
			return Common::ToDouble(record.value(), FIELD_VIEW_DISTANCE);
		}
		return std::nullopt;
	}

	std::optional<double> World::ReadDockDistance(int worldId)
	{
		Initialize();
		auto record = Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_DOCK_DISTANCE, worldId);
		if (record)
		{
			return Common::ToDouble(record.value(), FIELD_DOCK_DISTANCE);
		}
		return std::nullopt;
	}

	std::optional<double> World::ReadWindHeading(int worldId)
	{
		Initialize();
		auto record = Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_WIND_HEADING, worldId);
		if (record)
		{
			return Common::ToDouble(record.value(), FIELD_WIND_HEADING);
		}
		return std::nullopt;
	}

	std::optional<int> World::ReadCurrencyItemTypeId(int worldId)
	{
		Initialize();
		auto record = Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_CURRENCY_ITEM_SUBTYPE_ID, worldId);
		if (record)
		{
			return Common::ToInt(record.value(), FIELD_CURRENCY_ITEM_SUBTYPE_ID);
		}
		return std::nullopt;
	}

	std::optional<int> World::ReadDay(int worldId)
	{
		Initialize();
		auto record = Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_DAY, worldId);
		if (record)
		{
			return Common::ToInt(record.value(), FIELD_DAY);
		}
		return std::nullopt;
	}

	std::optional<int> World::ReadVersion(int worldId)
	{
		Initialize();
		auto record = Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_VERSION, worldId);
		if (record)
		{
			return Common::ToInt(record.value(), FIELD_VERSION);
		}
		return std::nullopt;
	}

	void World::WriteWindHeading(int worldId, double heading)
	{
		Initialize();
		Common::Execute(UPDATE_ITEM_COLUMN, FIELD_WIND_HEADING, heading, worldId);
	}

	void World::WriteDay(int worldId, int day)
	{
		Initialize();
		Common::Execute(UPDATE_ITEM_COLUMN, FIELD_DAY, day, worldId);
	}
}