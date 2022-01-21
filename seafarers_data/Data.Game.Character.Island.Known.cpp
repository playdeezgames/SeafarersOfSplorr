#include <algorithm>
#include "Data.Game.Character.h"
#include "Data.Game.Character.Island.Known.h"
#include "Data.Game.Common.h"
#include "Data.Game.Island.h"
namespace data::game::character::island
{
	using namespace std::string_view_literals;
	static constexpr std::string_view CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [CharacterKnownIslands]
		(
			[CharacterId],
			[IslandId],
			UNIQUE([CharacterId],[IslandId]),
			FOREIGN KEY ([CharacterId]) REFERENCES [Characters]([CharacterId]),
			FOREIGN KEY ([IslandId]) REFERENCES [Islands]([IslandId])
		);)"sv;
	static constexpr std::string_view REPLACE_ITEM =
		R"(REPLACE INTO [CharacterKnownIslands]
		(
			[CharacterId],
			[IslandId]
		)
		VALUES({},{});)"sv;
	static constexpr std::string_view QUERY_ITEM =
		R"(SELECT 
			[IslandId] 
		FROM [CharacterKnownIslands] 
		WHERE 
			[CharacterId]={} 
			AND [IslandId]={};)"sv;
	static constexpr std::string_view QUERY_FOR_CHARACTER =
		R"(SELECT 
			[IslandId] 
		FROM [CharacterKnownIslands] 
		WHERE 
			[CharacterId]={};)"sv;
	static constexpr std::string_view DELETE_ALL =
		R"(DELETE FROM [CharacterKnownIslands];)"sv;
	static constexpr std::string_view DELETE_CHARACTER =
		R"(DELETE FROM [CharacterKnownIslands] 
		WHERE 
			[CharacterId]={};)"sv;

	static constexpr std::string_view FIELD_ISLAND_ID = "IslandId";

	void Known::Initialize()
	{
		Character::Initialize();
		Island::Initialize();
		Common::Execute(CREATE_TABLE);
	}

	void Known::Write(int characterId, int islandId)
	{
		Initialize();
		Common::Execute(REPLACE_ITEM, characterId, islandId);
	}

	bool Known::Has(int characterId, int islandId)
	{
		Initialize();
		return Common::TryExecuteForOne(QUERY_ITEM, characterId, islandId).has_value();
	}

	void Known::Clear(int characterId)
	{
		Initialize();
		Common::Execute(DELETE_CHARACTER, characterId);
	}

	void Known::Clear()
	{
		Initialize();
		Common::Execute(DELETE_ALL);
	}

	std::vector<int> Known::All(int characterId)
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