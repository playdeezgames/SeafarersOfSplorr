#include <algorithm>
#include "Data.Game.Character.h"
#include "Data.Game.Character.Flags.h"
#include "Data.Game.Common.h"
namespace data::game::character
{
	using namespace std::string_view_literals;
	static constexpr std::string_view CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [CharacterFlags]
		(
			[CharacterId] INT NOT NULL,
			[Flag] INT NOT NULL,
			UNIQUE([CharacterId],[Flag])
		);)"sv;
	static constexpr std::string_view QUERY_ITEM = 
		R"(SELECT 
			[Flag] 
		FROM [CharacterFlags] 
		WHERE 
			[CharacterId]={} 
			AND [Flag]={};)"sv;
	static constexpr std::string_view REPLACE_ITEM = 
		R"(REPLACE INTO [CharacterFlags]
		(
			[CharacterId],
			[Flag]
		) 
		VALUES({},{});)"sv;
	static constexpr std::string_view DELETE_ITEM = 
		R"(DELETE FROM [CharacterFlags] 
		WHERE 
			[CharacterId]={} 
			AND [Flag]={};)"sv;
	static constexpr std::string_view QUERY_ALL_FOR_CHARACTER = 
		R"(SELECT 
			[Flag] 
		FROM [CharacterFlags] 
		WHERE 
			[CharacterId]={};)"sv;

	static constexpr std::string_view FIELD_FLAG = "Flag";

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