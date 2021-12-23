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
			[CounterId] INT NOT NULL,
			[Value] INT NOT NULL,
			UNIQUE([CharacterId],[CounterId]),
			FOREIGN KEY ([CharacterId]) REFERENCES [Characters]([CharacterId])
		);)"s;
	static const std::string REPLACE_ITEM = 
		R"(REPLACE INTO [CharacterCounters]
		(
			[CharacterId],
			[CounterId],
			[Value]
		) 
		VALUES({},{},{});)"s;
	static const std::string QUERY_ITEM = 
		R"(SELECT 
			[Value] 
		FROM [CharacterCounters] 
		WHERE 
			[CharacterId]={} 
			AND [CounterId]={};)"s;
	static const std::string DELETE_ITEM = 
		R"(DELETE FROM [CharacterCounters] 
		WHERE 
			[CharacterId]={} 
			AND [CounterId]={};)"s;

	static const std::string FIELD_VALUE = "Value";

	void Counter::Initialize()
	{
		Character::Initialize();
		Common::Execute(CREATE_TABLE);
	}

	void Counter::Write(int characterId, int counterId, int value)
	{
		Initialize();
		Common::Execute(REPLACE_ITEM, characterId, counterId, value);
	}

	std::optional<int> Counter::Read(int characterId, int counterId)
	{
		Initialize();
		auto records = Common::Execute(QUERY_ITEM, characterId, counterId);
		if (!records.empty())
		{
			return common::Data::ToOptionalInt(records.front()[FIELD_VALUE]);
		}
		return std::nullopt;
	}

	void Counter::Clear(int characterId, int counterId)
	{
		Initialize();
		Common::Execute(DELETE_ITEM, characterId, counterId);
	}
}