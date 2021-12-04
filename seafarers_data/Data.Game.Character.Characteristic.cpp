#include <Common.Data.h>
#include "Data.Game.Character.Characteristic.h"
#include "Data.Game.Common.h"
namespace data::game::character
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [Characteristics]([CharacterId] INT NOT NULL,[CharacteristicId] INT NOT NULL,[Value] INT NOT NULL,UNIQUE([CharacterId],[CharacteristicId]));";
	static const std::string QUERY_ITEM = "SELECT [Value] FROM [Characteristics] WHERE [CharacterId]={} AND [CharacteristicId]={};";
	static const std::string QUERY_ITEMS = "SELECT [CharacteristicId], [Value] FROM [Characteristics] WHERE [CharacterId]={};";
	static const std::string REPLACE_ITEM = "REPLACE INTO [Characteristics]([CharacterId],[CharacteristicId],[Value]) VALUES({},{},{});";

	static const std::string FIELD_VALUE = "Value";
	static const std::string FIELD_CHARACTERISTIC_ID = "CharacteristicId";

	static const auto AutoCreateTable = data::game::Common::Run(CREATE_TABLE);

	void Characteristic::Write(int characterId, int characteristicId, int value)
	{
		AutoCreateTable();
		Common::Execute(REPLACE_ITEM, characterId, characteristicId, value);
	}

	std::optional<int> Characteristic::Read(int characterId, int characteristicId)
	{
		AutoCreateTable();
		auto records = Common::Execute(QUERY_ITEM, characterId, characteristicId);
		if (!records.empty())
		{
			return common::Data::ToOptionalInt(records.front().find(FIELD_VALUE)->second);
		}
		return std::nullopt;
	}

	std::map<int, int> Characteristic::Read(int characterId)
	{
		AutoCreateTable();
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