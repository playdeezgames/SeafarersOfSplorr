#include <algorithm>
#include <Common.Data.h>
#include "Data.Game.Character.h"
#include "Data.Game.Common.h"
#include "Data.Game.Island.h"
#include "Data.Game.Tribe.h"
#include <iterator>
namespace data::game
{
	using namespace std::string_literals;
	static const std::string CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [Characters]
		(
			[CharacterId] INTEGER PRIMARY KEY AUTOINCREMENT,
			[Name] TEXT NOT NULL,
			[OriginIslandId] INT NOT NULL,
			[TribeId] INT NOT NULL,
			FOREIGN KEY ([OriginIslandId]) REFERENCES [Islands]([IslandId]),
			FOREIGN KEY ([TribeId]) REFERENCES [Tribes]([TribeId])
		);)"s;
	static const std::string QUERY_ITEM_COLUMN =
		R"(SELECT 
			[{}]
		FROM [Characters] 
		WHERE 
			[CharacterId] = {};)"s;
	static const std::string CREATE_ITEM =
		R"(INSERT INTO [Characters]
		(
			[Name],
			[OriginIslandId],
			[TribeId]
		) 
		VALUES({}, {}, {});)"s;
	static const std::string UPDATE_ITEM_COLUMN =
		R"(UPDATE [Characters] 
		SET 
			[{}]={}
		WHERE 
			CharacterId={};)"s;
	static const std::string QUERY_ALL =
		R"(SELECT 
			[CharacterId] 
		FROM [Characters];)"s;

	static const std::string FIELD_NAME = "Name";
	static const std::string FIELD_TRIBE_ID = "TribeId";
	static const std::string FIELD_ORIGIN_ISLAND_ID = "OriginIslandId";
	static const std::string FIELD_CHARACTER_ID = "CharacterId";

	void Character::Initialize()
	{
		Island::Initialize();
		Tribe::Initialize();
		Common::Execute(CREATE_TABLE);
	}

	int Character::Create(const std::string& name, int islandId, int tribeId)
	{
		Initialize();
		Common::Execute(
			CREATE_ITEM,
			common::Data::QuoteString(name),
			islandId,
			tribeId);
		return Common::LastInsertedIndex();
	}

	std::optional<int> Character::ReadOriginIslandId(int characterId)
	{
		Initialize();
		return Common::TryToInt(
			Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_ORIGIN_ISLAND_ID, characterId), 
			FIELD_ORIGIN_ISLAND_ID);
	}

	std::optional<int> Character::ReadTribeId(int characterId)
	{
		Initialize();
		return Common::TryToInt(
			Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_TRIBE_ID, characterId), 
			FIELD_TRIBE_ID);
	}

	std::optional<std::string> Character::ReadName(int characterId)
	{
		Initialize();
		return Common::TryToString(
			Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_NAME, characterId), 
			FIELD_NAME);
	}

	std::vector<int> Character::All()
	{
		Initialize();
		auto records = Common::Execute(QUERY_ALL);
		std::vector<int> result;
		std::transform(
			records.begin(),
			records.end(),
			std::back_inserter(result),
			Common::DoToInt(FIELD_CHARACTER_ID));
		return result;
	}
}