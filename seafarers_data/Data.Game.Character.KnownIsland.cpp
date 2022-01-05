#include <algorithm>
#include <Common.Data.h>
#include "Data.Game.Character.h"
#include "Data.Game.Character.KnownIsland.h"
#include "Data.Game.Common.h"
#include "Data.Game.Island.h"
#include <iterator>
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
			[CharacterId]={} 
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

	bool KnownIsland::Has(int characterId, int islandId)
	{
		Initialize();
		return Common::TryExecuteForOne(QUERY_ITEM, characterId, islandId).has_value();
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

	std::vector<int> KnownIsland::All(int characterId)
	{
		Initialize();
		auto records = Common::Execute(QUERY_FOR_CHARACTER, characterId);
		std::vector<int> result;
		std::transform(
			records.begin(),
			records.end(),
			std::back_inserter(result),
			Common::DoToInt(FIELD_ISLAND_ID));
		return result;
	}
}