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
			[WorldId] INT NOT NULL UNIQUE CHECK([WorldId]=1),
			[Difficulty] INT NOT NULL,
			[WindHeading] REAL NOT NULL,
			[CurrencyItemTypeId] INT NOT NULL,
			[Day] INT NOT NULL --changes each turn
		);)"s;
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
			[Difficulty],
			[WindHeading],
			[CurrencyItemTypeId],
			[Day]
		) 
		VALUES ({},{},{},{},{});)"s;

	static const std::string FIELD_DIFFICULTY = "Difficulty";
	static const std::string FIELD_WIND_HEADING = "WindHeading";
	static const std::string FIELD_CURRENCY_ITEM_TYPE_ID = "CurrencyItemTypeId";
	static const std::string FIELD_DAY = "Day";

	void World::Initialize()
	{
		Common::Execute(CREATE_TABLE);
	}

	void World::Write(
		int worldId,
		int difficulty,
		double windHeading,
		int currencyItemTypeId,
		int day)
	{
		Initialize();
		Common::Execute(
			REPLACE_ITEM,
			worldId,
			difficulty,
			windHeading,
			currencyItemTypeId,
			day);
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
		auto record = Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_CURRENCY_ITEM_TYPE_ID, worldId);
		if (record)
		{
			return Common::ToInt(record.value(), FIELD_CURRENCY_ITEM_TYPE_ID);
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

	std::optional<int> World::ReadDifficulty(int worldId)
	{
		Initialize();
		auto record = Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_DIFFICULTY, worldId);
		if (record)
		{
			return Common::ToInt(record.value(), FIELD_DIFFICULTY);
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