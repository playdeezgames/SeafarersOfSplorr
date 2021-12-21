#include <Common.Data.h>
#include "Data.Game.Character.h"
#include "Data.Game.Character.Characteristic.h"
#include "Data.Game.Common.h"
namespace data::game::character
{
	static const std::string CREATE_TABLE =
		"CREATE TABLE IF NOT EXISTS [CharacterCharacteristics]"
		"("
			"[CharacterId] INT NOT NULL,"
			"[CharacteristicId] INT NOT NULL,"
			"[Value] INT NOT NULL,"
			"UNIQUE([CharacterId],[CharacteristicId]),"
			"FOREIGN KEY ([CharacterId]) REFERENCES [Characters]([CharacterId])"
		");";
	static const std::string QUERY_ITEM = 
		"SELECT "
			"[Value] "
		"FROM [CharacterCharacteristics] "
		"WHERE "
			"[CharacterId]={} "
			"AND [CharacteristicId]={};";
	static const std::string QUERY_ITEMS = 
		"SELECT "
			"[CharacteristicId], "
			"[Value] "
		"FROM [CharacterCharacteristics] "
		"WHERE "
			"[CharacterId]={};";
	static const std::string REPLACE_ITEM = 
		"REPLACE INTO [CharacterCharacteristics]"
		"("
			"[CharacterId],"
			"[CharacteristicId],"
			"[Value]"
		") "
		"VALUES({},{},{});";

	static const std::string FIELD_VALUE = "Value";
	static const std::string FIELD_CHARACTERISTIC_ID = "CharacteristicId";

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
		auto records = Common::Execute(QUERY_ITEM, characterId, characteristicId);
		if (!records.empty())
		{
			return common::Data::ToOptionalInt(records.front().find(FIELD_VALUE)->second);
		}
		return std::nullopt;
	}

	std::map<int, int> Characteristic::Read(int characterId)
	{
		Initialize();
		std::map<int, int> result;
		auto records = Common::Execute(QUERY_ITEMS, characterId);
		for (auto record : records)
		{
			result
				[common::Data::ToInt(record.find(FIELD_CHARACTERISTIC_ID)->second)] = 
				common::Data::ToInt(record.find(FIELD_VALUE)->second);
		}
		return result;
	}

}