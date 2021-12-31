#include <Common.Data.h>
#include "Data.Game.Character.h"
#include "Data.Game.Character.Counter.h"
#include "Data.Game.Common.h"
namespace data::game::character
{
	using namespace std::string_literals;
	static const std::string CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [CharacterCounters]
		(
			[CharacterId] INT NOT NULL,
			[Counter] INT NOT NULL,
			[Value] INT NOT NULL,
			UNIQUE([CharacterId],[Counter]),
			FOREIGN KEY ([CharacterId]) REFERENCES [Characters]([CharacterId])
		);)"s;
	static const std::string REPLACE_ITEM = 
		R"(REPLACE INTO [CharacterCounters]
		(
			[CharacterId],
			[Counter],
			[Value]
		) 
		VALUES({},{},{});)"s;
	static const std::string QUERY_ITEM = 
		R"(SELECT 
			[Value] 
		FROM [CharacterCounters] 
		WHERE 
			[CharacterId]={} 
			AND [Counter]={};)"s;
	static const std::string DELETE_ITEM = 
		R"(DELETE FROM [CharacterCounters] 
		WHERE 
			[CharacterId]={} 
			AND [Counter]={};)"s;

	static const std::string FIELD_VALUE = "Value";

	void Counter::Initialize()
	{
		Character::Initialize();
		Common::Execute(CREATE_TABLE);
	}

	void Counter::Write(int characterId, int counter, int value)
	{
		Initialize();
		Common::Execute(REPLACE_ITEM, characterId, counter, value);
	}

	std::optional<int> Counter::Read(int characterId, int counter)
	{
		Initialize();
		auto record = Common::TryExecuteForOne(QUERY_ITEM, characterId, counter);
		if (record)
		{
			return Common::ToInt(*record, FIELD_VALUE);
		}
		return std::nullopt;
	}

	void Counter::Clear(int characterId, int counter)
	{
		Initialize();
		Common::Execute(DELETE_ITEM, characterId, counter);
	}
}