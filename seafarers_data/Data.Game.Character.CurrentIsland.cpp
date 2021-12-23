#include <Common.Data.h>
#include "Data.Game.Character.CurrentIsland.h"
#include "Data.Game.Common.h"
namespace data::game::character
{
	using namespace std::string_literals;
	static const std::string CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [CharacterCurrentIslands]
		(
			[CharacterId] INT NOT NULL UNIQUE,
			[IslandId] INT NOT NULL
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

	static const auto AutoCreateTable = data::game::Common::Run(CREATE_TABLE);

	std::optional<int> Island::Read(int characterId)
	{
		AutoCreateTable();
		auto records = Common::Execute(QUERY_ITEM, characterId);
		if (!records.empty())
		{
			return common::Data::ToOptionalInt(records.front()[FIELD_ISLAND_ID]);
		}
		return std::nullopt;
	}

	void Island::Write(int characterId, int islandId)
	{
		AutoCreateTable();
		Common::Execute(REPLACE_ITEM, characterId, islandId);
	}

	void Island::Clear(int characterId)
	{
		AutoCreateTable();
		Common::Execute(DELETE_ITEM, characterId);
	}

	std::list<int> Island::All(int islandId)
	{
		AutoCreateTable();
		auto records = Common::Execute(QUERY_ALL, islandId);
		std::list<int> result;
		for (auto record : records)
		{
			result.push_back(common::Data::ToInt(record.find(FIELD_CHARACTER_ID)->second));
		}
		return result;
	}
}