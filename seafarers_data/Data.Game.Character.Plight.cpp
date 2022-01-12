#include <algorithm>
#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Character.Plight.h"
#include "Data.Game.Character.h"
#include <iterator>
namespace data::game::character
{
	using namespace std::string_view_literals;
	static constexpr std::string_view CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [CharacterPlights]
		(
			[CharacterId] INT NOT NULL,
			[Plight] INT NOT NULL, 
			[Duration] INT NOT NULL, 
			UNIQUE([CharacterId],[Plight]),
			FOREIGN KEY ([CharacterId]) REFERENCES [Characters]([CharacterId])
		);)"sv;
	static constexpr std::string_view DELETE_ALL_FOR_CHARACTER =
		R"(DELETE FROM [CharacterPlights] 
		WHERE 
			[CharacterId]={};)"sv;
	static constexpr std::string_view DELETE_ALL_PLIGHTS =
		R"(DELETE FROM [CharacterPlights];)"sv;
	static constexpr std::string_view DELETE_ITEM =
		R"(DELETE FROM [CharacterPlights] 
		WHERE 
			[CharacterId]={} 
			AND [Plight]={};)"sv;
	static constexpr std::string_view REPLACE_ITEM =
		R"(REPLACE INTO [CharacterPlights]
		(
			[CharacterId], 
			[Plight], 
			[Duration]
		) 
		VALUES({}, {}, {});)"sv;
	static constexpr std::string_view QUERY_ITEM =
		R"(SELECT 
			[Duration] 
		FROM [CharacterPlights] 
		WHERE 
			[CharacterId]={} 
			AND [Plight]={};)"sv;
	static constexpr std::string_view QUERY_ALL =
		R"(SELECT 
			[Plight] 
		FROM [CharacterPlights] 
		WHERE 
			[CharacterId]={};)"sv;

	static constexpr std::string_view FIELD_PLIGHT = "Plight";
	static constexpr std::string_view FIELD_DURATION = "Duration";

	void Plight::ClearPlight(int characterId, int plight)
	{
		Initialize();
		Common::Execute(DELETE_ITEM, characterId, plight);

	}
	void Plight::Clear(int characterId)
	{
		Initialize();
		Common::Execute(DELETE_ALL_FOR_CHARACTER,characterId);
	}

	std::vector<int> Plight::All(int characterId)
	{
		Initialize();
		auto records = Common::Execute(QUERY_ALL, characterId);
		std::vector<int> result;
		std::transform(
			records.begin(),
			records.end(),
			std::back_inserter(result),
			Common::DoToInt(FIELD_PLIGHT));
		return result;
	}

	void Plight::ClearAll()
	{
		Initialize();
		Common::Execute(DELETE_ALL_PLIGHTS);
	}

	void Plight::Initialize()
	{
		Character::Initialize();
		Common::Execute(CREATE_TABLE);
	}

	void Plight::Write(int characterId, int plight, int duration)
	{
		Initialize();
		Common::Execute(REPLACE_ITEM, characterId, plight, duration);
	}

	std::optional<int> Plight::ReadDuration(int characterId, int plight)
	{
		Initialize();
		return Common::TryToInt(
			Common::TryExecuteForOne(QUERY_ITEM, characterId, plight), 
			FIELD_DURATION);
	}
}