#include <Common.Data.h>
#include "Data.Game.Character.Island.h"
#include "Data.Game.Common.h"
namespace data::game::character
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [CharacterIslands]([CharacterId] INT NOT NULL UNIQUE,[IslandId] INT NOT NULL);";
	static const std::string QUERY_ITEM = "SELECT [IslandId] FROM [CharacterIslands] WHERE [CharacterId]={};";
	static const std::string REPLACE_ITEM = "REPLACE INTO [CharacterIslands]([CharacterId], [IslandId]) VALUES({},{});";
	static const std::string DELETE_ITEM = "DELETE FROM [CharacterIslands] WHERE [CharacterId]={};";

	static const std::string FIELD_ISLAND_ID = "IslandId";

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
	}

	void Island::Clear(int characterId)
	{
		AutoCreateTable();
	}

}