#include <Common.Data.h>
#include "Data.Game.Character.h"
#include "Data.Game.Common.h"
namespace data::game
{
	using namespace std::string_literals;
	static const std::string CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [Characters]
		(
			[CharacterId] INTEGER PRIMARY KEY AUTOINCREMENT,
			[State] INT NOT NULL, 
			[Name] TEXT NOT NULL
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
			[Name]
		) 
		VALUES({}, {});)"s;
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
	static const std::string FIELD_CHARACTER_ID = "CharacterId";

	void Character::Initialize()
	{
		data::game::Common::Execute(CREATE_TABLE);
	}

	int Character::Create(int state, const std::string& name)
	{
		Initialize();
		Common::Execute(
			CREATE_ITEM,
			state,
			common::Data::QuoteString(name));
		return Common::LastInsertedIndex();
	}

	void Character::WriteState(int characterId, int state)
	{
		Initialize();
		Common::Execute(UPDATE_ITEM_COLUMN, FIELD_STATE, state);
	}

	std::optional<int> Character::ReadState(int characterId)
	{
		Initialize();
		auto record = Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_STATE, characterId);
		if (record)
		{
			return Common::ToInt(record.value(), FIELD_STATE);
		}
		return std::nullopt;
	}

	std::optional<std::string> Character::ReadName(int characterId)
	{
		Initialize();
		auto record = Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_NAME, characterId);
		if (record)
		{
			return Common::ToString(record.value(), FIELD_NAME);
		}
		return std::nullopt;
	}

	std::list<int> Character::All()
	{
		Initialize();
		auto records = Common::Execute(QUERY_ALL);
		std::list<int> result;
		for (auto& record : records)
		{
			result.push_back(common::Data::ToInt(record[FIELD_CHARACTER_ID]));
		}
		return result;
	}
}