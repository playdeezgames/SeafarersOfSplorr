#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.World.h"
#include "Data.Game.World.Month.h"
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
	static const std::string QUERY_ITEM =
		R"(SELECT 
			[Ordinal],
			[Name],
			[Days] 
		FROM [WorldMonths] 
		WHERE 
			[WorldId]={} 
			AND [Ordinal]={};)"s;
	static const std::string QUERY_ITEM_COLUMN =
		R"(SELECT 
			[{}]
		FROM [WorldMonths] 
		WHERE 
			[WorldId]={} 
			AND [Ordinal]={};)"s;
	static const std::string QUERY_ALL =
		R"(SELECT 
			[Ordinal],
			[Name],
			[Days] 
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

	static Month ToMonth(const Common::Record& record)
	{
		return 
		{
			common::Data::ToInt(record.find(FIELD_ORDINAL)->second),
			record.find(FIELD_NAME)->second,
			common::Data::ToInt(record.find(FIELD_DAYS)->second),
		};
	}

	std::optional<Month> Month::Read(int worldId, int ordinal)
	{
		Initialize();
		auto records = Common::Execute(QUERY_ITEM, worldId, ordinal);
		if (!records.empty())
		{
			return ToMonth(records.front());
		}
		return std::nullopt;
	}

	int Month::YearLength(int worldId)
	{
		Initialize();
		auto records = Common::Execute(QUERY_YEAR_LENGTH, worldId);
		return 
			common::Data::ToInt(
				records
				.front()
				.find(FIELD_YEAR_LENGTH)->second);
	}

	std::list<Month> Month::All(int worldId)
	{
		Initialize();
		std::list<Month> result;
		auto records = Common::Execute(QUERY_ALL, worldId);
		for (auto record : records)
		{
			result.push_back(ToMonth(record));
		}
		return result;
	}

	void Month::Clear(int worldId)
	{
		Initialize();
		Common::Execute(DELETE_ALL, worldId);
	}

	std::list<int> Month::AllOrdinals(int worldId)
	{
		Initialize();
		std::list<int> result;
		auto records = Common::Execute(QUERY_ALL, worldId);
		for (auto record : records)
		{
			result.push_back(Common::ToInt(record, FIELD_ORDINAL));
		}
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
		auto record = Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_NAME, worldId, ordinal);
		if (record)
		{
			return Common::ToString(record.value(), FIELD_NAME);
		}
		return std::nullopt;
	}

	std::optional<int> Month::ReadDays(int worldId, int ordinal)
	{
		Initialize();
		auto record = Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_DAYS, worldId, ordinal);
		if (record)
		{
			return Common::ToInt(record.value(), FIELD_DAYS);
		}
		return std::nullopt;
	}
}