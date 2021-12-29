#include <Common.Data.h>
#include "Data.Game.Character.h"
#include "Data.Game.Common.h"
#include "Data.Game.Island.h"
#include "Data.Game.Tribe.h"
namespace data::game
{
	using namespace std::string_literals;
	static const std::string CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [Characters]
		(
			[CharacterId] INTEGER PRIMARY KEY AUTOINCREMENT,
			[State] INT NOT NULL, 
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
			[State],
			[Name],
			[OriginIslandId],
			[TribeId]
		) 
		VALUES({}, {}, {}, {});)"s;
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

	static const std::string FIELD_STATE = "State";
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

	int Character::Create(state_t state, const name_t& name, islandid_t islandId, tribeid_t tribeId)
	{
		Initialize();
		Common::Execute(
			CREATE_ITEM,
			state,
			common::Data::QuoteString(name),
			islandId,
			tribeId);
		return Common::LastInsertedIndex();
	}

	void Character::WriteState(characterid_t characterId, state_t state)
	{
		Initialize();
		Common::Execute(UPDATE_ITEM_COLUMN, FIELD_STATE, state);
	}

	std::optional<Character::state_t> Character::ReadState(characterid_t characterId)
	{
		Initialize();
		auto record = Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_STATE, characterId);
		if (record)
		{
			return Common::ToInt(record.value(), FIELD_STATE);
		}
		return std::nullopt;
	}

	std::optional<Character::islandid_t> Character::ReadOriginIslandId(characterid_t characterId)
	{
		Initialize();
		auto record = Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_ORIGIN_ISLAND_ID, characterId);
		if (record)
		{
			return Common::ToInt(record.value(), FIELD_ORIGIN_ISLAND_ID);
		}
		return std::nullopt;
	}

	std::optional<Character::tribeid_t> Character::ReadTribeId(characterid_t characterId)
	{
		Initialize();
		auto record = Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_TRIBE_ID, characterId);
		if (record)
		{
			return Common::ToInt(record.value(), FIELD_TRIBE_ID);
		}
		return std::nullopt;
	}

	std::optional<Character::name_t> Character::ReadName(characterid_t characterId)
	{
		Initialize();
		auto record = Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_NAME, characterId);
		if (record)
		{
			return Common::ToString(record.value(), FIELD_NAME);
		}
		return std::nullopt;
	}

	std::list<Character::characterid_t> Character::All()
	{
		Initialize();
		auto records = Common::Execute(QUERY_ALL);
		std::list<characterid_t> result;
		for (auto& record : records)
		{
			result.push_back(common::Data::ToInt(record[FIELD_CHARACTER_ID]));
		}
		return result;
	}
}