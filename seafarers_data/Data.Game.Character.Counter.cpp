#include <Common.Data.h>
#include "Data.Game.Character.h"
#include "Data.Game.Character.Counter.h"
#include "Data.Game.Common.h"
namespace data::game::character
{
	using namespace std::string_view_literals;
	static constexpr std::string_view CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [CharacterCounters]
		(
			[CharacterId] INT NOT NULL,
			[Counter] INT NOT NULL,
			[Value] INT NOT NULL,
			UNIQUE([CharacterId],[Counter]),
			FOREIGN KEY ([CharacterId]) REFERENCES [Characters]([CharacterId])
		);)"sv;
	static constexpr std::string_view REPLACE_ITEM = 
		R"(REPLACE INTO [CharacterCounters]
		(
			[CharacterId],
			[Counter],
			[Value]
		) 
		VALUES({},{},{});)"sv;
	static constexpr std::string_view QUERY_ITEM = 
		R"(SELECT 
			[Value] 
		FROM [CharacterCounters] 
		WHERE 
			[CharacterId]={} 
			AND [Counter]={};)"sv;
	static constexpr std::string_view DELETE_ITEM = 
		R"(DELETE FROM [CharacterCounters] 
		WHERE 
			[CharacterId]={} 
			AND [Counter]={};)"sv;

	static constexpr std::string_view FIELD_VALUE = "Value";

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
		return Common::TryToInt(
			Common::TryExecuteForOne(QUERY_ITEM, characterId, counter), 
			FIELD_VALUE);
	}

	void Counter::Clear(int characterId, int counter)
	{
		Initialize();
		Common::Execute(DELETE_ITEM, characterId, counter);
	}
}