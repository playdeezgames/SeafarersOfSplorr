#include <Common.Data.h>
#include "Data.Game.Character.h"
#include "Data.Game.Island.h"
#include "Data.Game.Character.CurrentIsland.h"
#include "Data.Game.Common.h"
namespace data::game::character
{
	using namespace std::string_literals;
	static const std::string CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [CharacterCurrentIslands]
		(
			[CharacterId] INT NOT NULL UNIQUE,
			[IslandId] INT NOT NULL,
			FOREIGN KEY ([CharacterId]) REFERENCES [Characters]([CharacterId]),
			FOREIGN KEY ([IslandId]) REFERENCES [Islands]([IslandId])
		);)"s;
	static const std::string QUERY_ITEM = 
		R"(SELECT 
			[IslandId] 
		FROM [CharacterCurrentIslands] 
		WHERE 
			[CharacterId]={};)"s;
	static const std::string QUERY_ALL = 
		R"(SELECT 
			[CharacterId] 
		FROM [CharacterCurrentIslands] 
		WHERE 
			[IslandId]={};)"s;
	static const std::string REPLACE_ITEM = 
		R"(REPLACE INTO [CharacterCurrentIslands]
		(
			[CharacterId], 
			[IslandId]
		) 
		VALUES({},{});)"s;
	static const std::string DELETE_ITEM = 
		R"(DELETE FROM[CharacterCurrentIslands] 
		WHERE 
			[CharacterId]={};)"s;

	static const std::string FIELD_ISLAND_ID = "IslandId";
	static const std::string FIELD_CHARACTER_ID = "CharacterId";

	void CurrentIsland::Initialize()
	{
		Character::Initialize();
		Island::Initialize();
		Common::Execute(CREATE_TABLE);
	}

	std::optional<CurrentIsland::islandid_t> CurrentIsland::Read(characterid_t characterId)
	{
		Initialize();
		auto records = Common::Execute(QUERY_ITEM, characterId);
		if (!records.empty())
		{
			return common::Data::ToOptionalInt(records.front()[FIELD_ISLAND_ID]);
		}
		return std::nullopt;
	}

	void CurrentIsland::Write(characterid_t characterId, islandid_t islandId)
	{
		Initialize();
		Common::Execute(REPLACE_ITEM, characterId, islandId);
	}

	void CurrentIsland::Clear(characterid_t characterId)
	{
		Initialize();
		Common::Execute(DELETE_ITEM, characterId);
	}

	std::list<CurrentIsland::characterid_t> CurrentIsland::All(islandid_t islandId)
	{
		Initialize();
		auto records = Common::Execute(QUERY_ALL, islandId);
		std::list<characterid_t> result;
		for (auto record : records)
		{
			result.push_back(common::Data::ToInt(record.find(FIELD_CHARACTER_ID)->second));
		}
		return result;
	}
}