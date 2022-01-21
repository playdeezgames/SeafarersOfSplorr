#include <algorithm>
#include "Data.Game.Character.h"
#include "Data.Game.Island.h"
#include "Data.Game.Character.Island.Current.h"
#include "Data.Game.Common.h"
namespace data::game::character::island
{
	using namespace std::string_view_literals;
	static constexpr std::string_view CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [CharacterCurrentIslands]
		(
			[CharacterId] INT NOT NULL UNIQUE,
			[IslandId] INT NOT NULL,
			FOREIGN KEY ([CharacterId]) REFERENCES [Characters]([CharacterId]),
			FOREIGN KEY ([IslandId]) REFERENCES [Islands]([IslandId])
		);)"sv;
	static constexpr std::string_view QUERY_ITEM = 
		R"(SELECT 
			[IslandId] 
		FROM [CharacterCurrentIslands] 
		WHERE 
			[CharacterId]={};)"sv;
	static constexpr std::string_view QUERY_ALL = 
		R"(SELECT 
			[CharacterId] 
		FROM [CharacterCurrentIslands] 
		WHERE 
			[IslandId]={};)"sv;
	static constexpr std::string_view REPLACE_ITEM = 
		R"(REPLACE INTO [CharacterCurrentIslands]
		(
			[CharacterId], 
			[IslandId]
		) 
		VALUES({},{});)"sv;
	static constexpr std::string_view DELETE_ITEM = 
		R"(DELETE FROM[CharacterCurrentIslands] 
		WHERE 
			[CharacterId]={};)"sv;

	static constexpr std::string_view FIELD_ISLAND_ID = "IslandId";
	static constexpr std::string_view FIELD_CHARACTER_ID = "CharacterId";

	void Current::Initialize()
	{
		Character::Initialize();
		Island::Initialize();
		Common::Execute(CREATE_TABLE);
	}

	std::optional<int> Current::Read(int characterId)
	{
		Initialize();
		return Common::TryToInt(
			Common::TryExecuteForOne(QUERY_ITEM, characterId),
			FIELD_ISLAND_ID);
	}

	void Current::Write(int characterId, int islandId)
	{
		Initialize();
		Common::Execute(REPLACE_ITEM, characterId, islandId);
	}

	void Current::Clear(int characterId)
	{
		Initialize();
		Common::Execute(DELETE_ITEM, characterId);
	}

	std::vector<int> Current::All(int islandId)
	{
		Initialize();
		auto records = Common::Execute(QUERY_ALL, islandId);
		std::vector<int> result;
		std::transform(
			records.begin(),
			records.end(),
			std::back_inserter(result),
			Common::DoToInt(FIELD_ISLAND_ID));
		return result;
	}
}