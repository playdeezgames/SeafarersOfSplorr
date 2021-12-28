#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Character.Plight.h"
#include "Data.Game.Character.h"
namespace data::game::character
{
	using namespace std::string_literals;
	static const std::string CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [CharacterPlights]
		(
			[CharacterId] INT NOT NULL,
			[Plight] INT NOT NULL, 
			[Duration] INT NOT NULL, 
			UNIQUE([CharacterId],[Plight]),
			FOREIGN KEY ([CharacterId]) REFERENCES [Characters]([CharacterId])
		);)"s;
	static const std::string DELETE_ALL_FOR_CHARACTER = 
		R"(DELETE FROM [CharacterPlights] 
		WHERE 
			[CharacterId]={};)"s;
	static const std::string DELETE_ALL_PLIGHTS = 
		R"(DELETE FROM [CharacterPlights];)"s;
	static const std::string DELETE_ITEM = 
		R"(DELETE FROM [CharacterPlights] 
		WHERE 
			[CharacterId]={} 
			AND [Plight]={};)"s;
	static const std::string REPLACE_ITEM = 
		R"(REPLACE INTO [CharacterPlights]
		(
			[CharacterId], 
			[Plight], 
			[Duration]
		) 
		VALUES({}, {}, {});)"s;
	static const std::string QUERY_ITEM = 
		R"(SELECT 
			[Duration] 
		FROM [CharacterPlights] 
		WHERE 
			[CharacterId]={} 
			AND [Plight]={};)"s;
	static const std::string QUERY_ALL = 
		R"(SELECT 
			[Plight] 
		FROM [CharacterPlights] 
		WHERE 
			[CharacterId]={};)"s;

	static const std::string FIELD_PLIGHT_ID = "PlightId";
	static const std::string FIELD_DURATION = "Duration";

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

	std::list<int> Plight::All(int characterId)
	{
		Initialize();
		std::list<int> result;
		auto records = Common::Execute(QUERY_ALL, characterId);
		for (auto& record : records)
		{
			result.push_back(Common::ToInt(record, FIELD_PLIGHT_ID));
		}
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
		auto record = Common::TryExecuteForOne(QUERY_ITEM, characterId, plight);
		if (record)
		{
			return Common::ToInt(record.value(), FIELD_DURATION);
		}
		return std::nullopt;

	}

}