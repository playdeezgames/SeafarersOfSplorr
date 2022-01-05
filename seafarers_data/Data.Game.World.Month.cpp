#include <algorithm>
#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.World.h"
#include "Data.Game.World.Month.h"
#include <iterator>
namespace data::game::world
{
	using namespace std::string_literals;
	static const std::string CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [WorldMonths]
		(
			[WorldId] INT NOT NULL,
			[Ordinal] INT NOT NULL,
			[Name] TEXT NOT NULL UNIQUE,
			[Days] INT NOT NULL,
			UNIQUE([WorldId],[Ordinal]),
			FOREIGN KEY ([WorldId]) REFERENCES [Worlds]([WorldId])
		);)"s;
	static const std::string REPLACE_ITEM =
		R"(REPLACE INTO [WorldMonths]
		(
			[WorldId],
			[Ordinal],
			[Name],
			[Days]
		) 
		VALUES({},{},{},{});)"s;
	static const std::string QUERY_ITEM_COLUMN =
		R"(SELECT 
			[{}]
		FROM [WorldMonths] 
		WHERE 
			[WorldId]={} 
			AND [Ordinal]={};)"s;
	static const std::string QUERY_ALL =
		R"(SELECT 
			[Ordinal]
		FROM [WorldMonths] 
		WHERE 
			[WorldId]={} 
		ORDER BY 
			[Ordinal];)"s;
	static const std::string QUERY_YEAR_LENGTH =
		R"(SELECT 
			SUM([Days]) AS [YearLength] 
		FROM [WorldMonths] 
		WHERE 
			[WorldId]={};)"s;
	static const std::string DELETE_ALL =
		R"(DELETE FROM [WorldMonths] 
		WHERE 
			[WorldId]={};)"s;

	static const std::string FIELD_ORDINAL = "Ordinal";
	static const std::string FIELD_NAME = "Name";
	static const std::string FIELD_DAYS = "Days";
	static const std::string FIELD_YEAR_LENGTH = "YearLength";

	void Month::Initialize()
	{
		World::Initialize();
		Common::Execute(CREATE_TABLE);
	}

	int Month::YearLength(int worldId)
	{
		Initialize();
		return Common::ToInt(
			Common::TryExecuteForOne(QUERY_YEAR_LENGTH, worldId).value(), 
			FIELD_YEAR_LENGTH);
	}

	void Month::Clear(int worldId)
	{
		Initialize();
		Common::Execute(DELETE_ALL, worldId);
	}

	std::vector<int> Month::AllOrdinals(int worldId)
	{
		Initialize();
		auto records = Common::Execute(QUERY_ALL, worldId);
		std::vector<int> result;
		std::transform(
			records.begin(),
			records.end(),
			std::back_inserter(result),
			Common::DoToInt(FIELD_ORDINAL));
		return result;
	}

	void Month::Write(int worldId, int ordinal, const std::string& name, int days)
	{
		Initialize();
		Common::Execute(
			REPLACE_ITEM,
			worldId,
			ordinal,
			common::Data::QuoteString(name),
			days);
	}

	std::optional<std::string> Month::ReadName(int worldId, int ordinal)
	{
		Initialize();
		return Common::TryToString(
			Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_NAME, worldId, ordinal),
			FIELD_NAME);
	}

	std::optional<int> Month::ReadDays(int worldId, int ordinal)
	{
		Initialize();
		return Common::TryToInt(
			Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_DAYS, worldId, ordinal),
			FIELD_DAYS);
	}
}