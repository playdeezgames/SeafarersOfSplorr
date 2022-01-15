#include "Data.Game.Character.h"
#include "Data.Game.Character.Island.Reputation.h"
#include "Data.Game.Common.h"
#include "Data.Game.Island.h"
namespace data::game::character::island
{
	using namespace std::string_view_literals;
	static constexpr auto CREATE_TABLE =
		R"(CREATE TABLE IF NOT EXISTS [CharacterIslandReputations]
		(
			[CharacterId] INT NOT NULL,
			[IslandId] INT NOT NULL,
			[Reputation] INT NOT NULL,
			UNIQUE([CharacterId],[IslandId]),
			FOREIGN KEY ([CharacterId]) REFERENCES [Characters]([CharacterId]),
			FOREIGN KEY ([IslandId]) REFERENCES [Islands]([IslandId])
		);)"sv;
	static constexpr auto QUERY_ITEM =
		R"(SELECT 
			[Reputation] 
		FROM [CharacterIslandReputations] 
		WHERE 
			[CharacterId]={} 
			AND [IslandId]={};)"sv;
	static constexpr auto REPLACE_ITEM =
		R"(REPLACE INTO [CharacterIslandReputations]
		(
			[CharacterId],
			[IslandId],
			[Reputation]
		) 
		VALUES({},{},{});)"sv;
	static constexpr auto FIELD_REPUTATION = "Reputation"sv;

	void Reputation::Initialize()
	{
		data::game::Character::Initialize();
		data::game::Island::Initialize();
		Common::Execute(CREATE_TABLE);
	}

	std::optional<int> Reputation::Read(int characterId, int islandId)
	{
		Initialize();
		return Common::TryToInt(
			Common::TryExecuteForOne(QUERY_ITEM, characterId, islandId),
			FIELD_REPUTATION);
	}

	void Reputation::Write(int characterId, int islandId, int reputation)
	{
		Initialize();
		Common::Execute(REPLACE_ITEM, characterId, islandId, reputation);
	}
}