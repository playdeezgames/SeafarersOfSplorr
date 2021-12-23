#include <Common.Data.h>
#include "Data.Game.Character.h"
#include "Data.Game.Character.KnownIsland.h"
#include "Data.Game.Common.h"
#include "Data.Game.Island.h"
namespace data::game::character
{
	using namespace std::string_literals;
	static const std::string CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [CharacterKnownIslands]
		(
			[CharacterId],
			[IslandId],
			UNIQUE([CharacterId],[IslandId]),
			FOREIGN KEY ([CharacterId]) REFERENCES [Characters]([CharacterId]),
			FOREIGN KEY ([IslandId]) REFERENCES [Islands]([IslandId])
		);)"s;
	static const std::string REPLACE_ITEM =
		R"(REPLACE INTO [CharacterKnownIslands]
		(
			[CharacterId],
			[IslandId]
		)
		VALUES({},{});)"s;
	static const std::string QUERY_ITEM =
		R"(SELECT 
			[IslandId] 
		FROM [CharacterKnownIslands] 
		WHERE 
			[CharacterId]={} "
			AND [IslandId]={};)"s;
	static const std::string QUERY_FOR_CHARACTER =
		R"(SELECT 
			[IslandId] 
		FROM [CharacterKnownIslands] 
		WHERE 
			[CharacterId]={};)"s;
	static const std::string DELETE_ALL =
		R"(DELETE FROM [CharacterKnownIslands];)"s;
	static const std::string DELETE_CHARACTER =
		R"(DELETE FROM [CharacterKnownIslands] 
		WHERE 
			[CharacterId]={};)"s;

	static const std::string FIELD_ISLAND_ID = "IslandId";

	void KnownIsland::Initialize()
	{
		Character::Initialize();
		Island::Initialize();
		Common::Execute(CREATE_TABLE);
	}

	void KnownIsland::Write(int characterId, int islandId)
	{
		Initialize();
		Common::Execute(REPLACE_ITEM, characterId, islandId);
	}

	bool KnownIsland::Read(int characterId, int islandId)
	{
		Initialize();
		return !Common::Execute(QUERY_ITEM, characterId, islandId).empty();
	}

	void KnownIsland::Clear(int characterId)
	{
		Initialize();
		Common::Execute(DELETE_CHARACTER, characterId);
	}

	void KnownIsland::Clear()
	{
		Initialize();
		Common::Execute(DELETE_ALL);
	}

	std::list<int> KnownIsland::All(int characterId)
	{
		Initialize();
		auto records = Common::Execute(QUERY_FOR_CHARACTER, characterId);
		std::list<int> result;
		for (auto record : records)
		{
			result.push_back(common::Data::ToInt(record.find(FIELD_ISLAND_ID)->second));
		}
		return result;
	}
}