#include <algorithm>
#include "Data.Game.Common.h"
#include "Data.Game.Island.h"
#include "Data.Game.Island.Tribe.h"
#include "Data.Game.Tribe.h"
namespace data::game::island
{
	using namespace std::string_view_literals;
	static constexpr std::string_view CREATE_TABLE =
		R"(CREATE TABLE IF NOT EXISTS [IslandTribes]
		(
			[IslandId] INT NOT NULL,
			[TribeId] INT NOT NULL,
			[Presence] INT,
			UNIQUE([IslandId],[TribeId]),
			FOREIGN KEY ([IslandId]) REFERENCES [Islands]([IslandId]),
			FOREIGN KEY ([TribeId]) REFERENCES [Tribes]([TribeId])
		);)"sv;
	static constexpr std::string_view DELETE_ALL =
		R"(DELETE FROM [IslandTribes];)"sv;
	static constexpr std::string_view REPLACE_ITEM =
		R"(REPLACE INTO [IslandTribes]
		(
			[IslandId],
			[TribeId],
			[Presence]
		) 
		VALUES({},{},{});)"sv;
	static constexpr std::string_view QUERY_ITEM =
		R"(SELECT 
			[Presence] 
		FROM [IslandTribes] 
		WHERE 
			[IslandId]={} 
			AND [TribeId]={};)"sv;
	static constexpr std::string_view QUERY_ALL =
		R"(SELECT 
			[TribeId],
			[Presence] 
		FROM [IslandTribes] 
		WHERE 
			[IslandId]={};)"sv;
	static constexpr std::string_view QUERY_ALL_TOTALS =
		R"(SELECT 
			[IslandId],
			SUM([Presence]) AS [TotalPresence] 
		FROM [IslandTribes] 
		GROUP BY 
			[IslandId];)"sv;

	static constexpr std::string_view FIELD_PRESENCE = "Presence";
	static constexpr std::string_view FIELD_TRIBE_ID = "TribeId";
	static constexpr std::string_view FIELD_ISLAND_ID = "IslandId";
	static constexpr std::string_view FIELD_TOTAL_PRESENCE = "TotalPresence";

	void Tribe::Initialize()
	{
		Island::Initialize();
		data::game::Tribe::Initialize();
		Common::Execute(CREATE_TABLE);
	}

	void Tribe::Clear()
	{
		Initialize();
		Common::Execute(DELETE_ALL);
	}

	void Tribe::Write(int islandId, int tribeId, int presence)
	{
		Initialize();
		Common::Execute(REPLACE_ITEM, islandId, tribeId, presence);
	}

	std::optional<size_t> Tribe::ReadPresence(int islandId, int tribeId)
	{
		Initialize();
		auto record = Common::TryExecuteForOne(QUERY_ITEM, islandId, tribeId);
		if (record)
		{
			return (size_t)Common::ToInt(record.value(), FIELD_PRESENCE);
		}
		return std::nullopt;
	}

	std::map<int, size_t> Tribe::All(int islandId)
	{
		Initialize();
		std::map<int, size_t> result;
		auto records = Common::Execute(QUERY_ALL, islandId);
		std::transform(
			records.begin(),
			records.end(),
			std::inserter(result, result.end()),
			[](const Common::Record& record) 
			{
				return std::make_pair(
					Common::ToInt(record, FIELD_TRIBE_ID),
					(size_t)Common::ToInt(record, FIELD_PRESENCE));
			});
		return result;
	}

	std::map<int, size_t> Tribe::AllTotals()
	{
		Initialize();
		std::map<int, size_t> result;
		auto records = Common::Execute(QUERY_ALL_TOTALS);
		std::transform(
			records.begin(),
			records.end(),
			std::inserter(result, result.end()),
			[](const Common::Record& record) 
			{
				return std::make_pair(Common::ToInt(record, FIELD_ISLAND_ID), (size_t)Common::ToInt(record, FIELD_TOTAL_PRESENCE));
			});
		return result;
	}
}