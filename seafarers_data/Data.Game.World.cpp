#include <Common.Data.h>
#include <Common.RNG.h>
#include "Data.Game.Common.h"
#include "Data.Game.World.h"
namespace data::game
{
	using namespace std::string_view_literals;
	static constexpr std::string_view CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [Worlds]
		(
			[WorldId] INT NOT NULL UNIQUE CHECK([WorldId]=1),
			[Difficulty] INT NOT NULL,
			[WindHeading] REAL NOT NULL,
			[CurrencyItemTypeId] INT NOT NULL,
			[Day] INT NOT NULL --changes each turn
		);)"sv;
	static constexpr std::string_view QUERY_ITEM_COLUMN =
		R"(SELECT 
			[{}] 
		FROM [Worlds] 
		WHERE 
			[WorldId]={};)"sv;
	static constexpr std::string_view UPDATE_ITEM_COLUMN =
		R"(UPDATE [Worlds] 
		SET 
			[{}]={} 
		WHERE 
			[WorldId]={};)"sv;
	static constexpr std::string_view REPLACE_ITEM =
		R"(REPLACE INTO [Worlds]
		(
			[WorldId],
			[Difficulty],
			[WindHeading],
			[CurrencyItemTypeId],
			[Day]
		) 
		VALUES ({},{},{},{},{});)"sv;

	static constexpr std::string_view FIELD_DIFFICULTY = "Difficulty";
	static constexpr std::string_view FIELD_WIND_HEADING = "WindHeading";
	static constexpr std::string_view FIELD_CURRENCY_ITEM_TYPE_ID = "CurrencyItemTypeId";
	static constexpr std::string_view FIELD_DAY = "Day";

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
		return Common::TryToDouble(
			Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_WIND_HEADING, worldId), 
			FIELD_WIND_HEADING);
	}

	std::optional<int> World::ReadCurrencyItemTypeId(int worldId)
	{
		Initialize();
		return Common::TryToInt(
			Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_CURRENCY_ITEM_TYPE_ID, worldId), 
			FIELD_CURRENCY_ITEM_TYPE_ID);
	}

	std::optional<int> World::ReadDay(int worldId)
	{
		Initialize();
		return Common::TryToInt(
			Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_DAY, worldId), 
			FIELD_DAY);
	}

	std::optional<int> World::ReadDifficulty(int worldId)
	{
		Initialize();
		return Common::TryToInt(
			Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_DIFFICULTY, worldId), 
			FIELD_DIFFICULTY);
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