#include <Common.Data.h>
#include "Data.Game.Character.Island.h"
#include "Data.Game.Common.h"
namespace data::game::character
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [CharacterIslands]([CharacterId] INT NOT NULL UNIQUE,[IslandId] INT NOT NULL);";
	static const std::string QUERY_ITEM = "SELECT [IslandId] FROM [CharacterIslands] WHERE [CharacterId]={};";
	static const std::string QUERY_ALL = "SELECT [CharacterId] FROM [CharacterIslands] WHERE [IslandId]={};";
	static const std::string REPLACE_ITEM = "REPLACE INTO [CharacterIslands]([CharacterId], [IslandId]) VALUES({},{});";
	static const std::string DELETE_ITEM = "DELETE FROM [CharacterIslands] WHERE [CharacterId]={};";

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