#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Island.h"
#include "Data.Game.Island.Tribe.h"
#include "Data.Game.Tribe.h"
namespace data::game::island
{
	using namespace std::string_literals;
	static const std::string CREATE_TABLE =
		R"(CREATE TABLE IF NOT EXISTS [IslandTribes]
		(
			[IslandId] INT NOT NULL,
			[TribeId] INT NOT NULL,
			[Presence] INT,
			UNIQUE([IslandId],[TribeId]),
			FOREIGN KEY ([IslandId]) REFERENCES [Islands]([IslandId]),
			FOREIGN KEY ([TribeId]) REFERENCES [Tribes]([TribeId])
		);)"s;
	static const std::string DELETE_ALL =
		R"(DELETE FROM [IslandTribes];)"s;
	static const std::string REPLACE_ITEM =
		R"(REPLACE INTO [IslandTribes]
		(
			[IslandId],
			[TribeId],
			[Presence]
		) 
		VALUES({},{},{});)";
	static const std::string QUERY_ITEM =
		R"(SELECT 
			[Presence] 
		FROM [IslandTribes] 
		WHERE 
			[IslandId]={} 
			AND [TribeId]={};)";
	static const std::string QUERY_ALL =
		R"(SELECT 
			[TribeId],
			[Presence] 
		FROM [IslandTribes] 
		WHERE 
			[IslandId]={};)"s;

	static const std::string FIELD_PRESENCE = "Presence";
	static const std::string FIELD_TRIBE_ID = "TribeId";

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

	std::optional<int> Tribe::Read(int islandId, int tribeId)
	{
		Initialize();
		auto record = Common::TryExecuteForOne(QUERY_ITEM, islandId, tribeId);
		if (record)
		{
			return Common::ToInt(record.value(), FIELD_PRESENCE);
		}
		return std::nullopt;
	}

	std::map<int, int> Tribe::All(int islandId)
	{
		Initialize();
		std::map<int, int> result;
		auto records = Common::Execute(QUERY_ALL, islandId);
		for (auto record : records)
		{
			result[Common::ToInt(record, FIELD_TRIBE_ID)] = Common::ToInt(record, FIELD_PRESENCE);
		}
		return result;
	}
}