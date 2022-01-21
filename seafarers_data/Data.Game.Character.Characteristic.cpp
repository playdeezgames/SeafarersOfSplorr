#include <algorithm>
#include "Data.Game.Character.h"
#include "Data.Game.Character.Characteristic.h"
#include "Data.Game.Common.h"
namespace data::game::character
{
	using namespace std::string_view_literals;
	static constexpr std::string_view CREATE_TABLE =
		R"(CREATE TABLE IF NOT EXISTS [CharacterCharacteristics]
		(
			[CharacterId] INT NOT NULL,
			[Characteristic] INT NOT NULL,
			[Value] INT NOT NULL,
			UNIQUE([CharacterId],[Characteristic]),
			FOREIGN KEY ([CharacterId]) REFERENCES [Characters]([CharacterId])
		);)"sv;
	static constexpr std::string_view QUERY_ITEM = 
		R"(SELECT 
			[Value] 
		FROM [CharacterCharacteristics] 
		WHERE 
			[CharacterId]={} 
			AND [Characteristic]={};)"sv;
	static constexpr std::string_view QUERY_ITEMS = 
		R"(SELECT 
			[Characteristic], 
			[Value] 
		FROM [CharacterCharacteristics] 
		WHERE 
			[CharacterId]={};)"sv;
	static constexpr std::string_view REPLACE_ITEM = 
		R"(REPLACE INTO [CharacterCharacteristics]
		(
			[CharacterId],
			[Characteristic],
			[Value]
		) 
		VALUES({},{},{});)"sv;

	static constexpr std::string_view FIELD_VALUE = "Value";
	static constexpr std::string_view FIELD_CHARACTERISTIC = "Characteristic";

	void Characteristic::Initialize()
	{
		Character::Initialize();
		Common::Execute(CREATE_TABLE);
	}

	void Characteristic::Write(int characterId, int characteristicId, int value)
	{
		Initialize();
		Common::Execute(REPLACE_ITEM, characterId, characteristicId, value);
	}

	std::optional<int> Characteristic::Read(int characterId, int characteristicId)
	{
		Initialize();
		return Common::TryToInt(
			Common::TryExecuteForOne(QUERY_ITEM, characterId, characteristicId),
			FIELD_VALUE);
	}

	std::map<int, int>
		Characteristic::Read(int characterId)
	{
		Initialize();
		std::map<int, int> result;
		auto records = Common::Execute(QUERY_ITEMS, characterId);
		std::transform(
			records.begin(),
			records.end(),
			std::inserter(result, result.end()),
			[](const Common::Record& record) 
			{
				return std::make_pair(Common::ToInt(record, FIELD_CHARACTERISTIC), Common::ToInt(record, FIELD_VALUE));
			}
		);
		return result;
	}

}