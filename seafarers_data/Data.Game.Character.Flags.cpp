#include <algorithm>
#include <Common.Data.h>
#include "Data.Game.Character.h"
#include "Data.Game.Character.Flags.h"
#include "Data.Game.Common.h"
#include <iterator>
namespace data::game::character
{
	using namespace std::string_literals;
	const static std::string CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [CharacterFlags]
		(
			[CharacterId] INT NOT NULL,
			[Flag] INT NOT NULL,
			UNIQUE([CharacterId],[Flag])
		);)"s;
	const static std::string QUERY_ITEM = 
		R"(SELECT 
			[Flag] 
		FROM [CharacterFlags] 
		WHERE 
			[CharacterId]={} 
			AND [Flag]={};)"s;
	const static std::string REPLACE_ITEM = 
		R"(REPLACE INTO [CharacterFlags]
		(
			[CharacterId],
			[Flag]
		) 
		VALUES({},{});)"s;
	const static std::string DELETE_ITEM = 
		R"(DELETE FROM [CharacterFlags] 
		WHERE 
			[CharacterId]={} 
			AND [Flag]={};)"s;
	const static std::string QUERY_ALL_FOR_CHARACTER = 
		R"(SELECT 
			[Flag] 
		FROM [CharacterFlags] 
		WHERE 
			[CharacterId]={};)"s;

	const static std::string FIELD_FLAG = "Flag";

	void Flags::Initialize()
	{
		Character::Initialize();
		Common::Execute(CREATE_TABLE);
	}

	bool Flags::Has(int characterId, int flagId)
	{
		Initialize();
		return Common::TryExecuteForOne(QUERY_ITEM, characterId, flagId).has_value();
	}

	void Flags::Write(int characterId, int flagId)
	{
		Initialize();
		Common::Execute(REPLACE_ITEM, characterId, flagId);
	}

	void Flags::Clear(int characterId, int flagId)
	{
		Initialize();
		Common::Execute(DELETE_ITEM, characterId, flagId);
	}

	std::set<int> Flags::All(int characterId)
	{
		Initialize();
		auto records = Common::Execute(QUERY_ALL_FOR_CHARACTER, characterId);
		std::set<int> result;
		std::transform(
			records.begin(),
			records.end(),
			std::inserter(result, result.end()),
			Common::DoToInt(FIELD_FLAG));
		return result;
	}
}