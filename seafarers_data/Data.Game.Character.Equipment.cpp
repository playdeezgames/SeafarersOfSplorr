#include <Common.Data.h>
#include "Data.Game.Character.Equipment.h"
#include "Data.Game.Common.h"
namespace data::game::character
{
	static const std::string CREATE_TABLE = 
		"CREATE TABLE IF NOT EXISTS [CharacterEquipment]"
		"("
			"[CharacterId] INT NOT NULL,"
			"[EquipSlotId] INT NOT NULL,"
			"[ItemId] INT NOT NULL,"
			"UNIQUE([CharacterId],[EquipSlotId])"
		");";
	static const std::string QUERY_ITEM = 
		"SELECT "
			"[ItemId] "
		"FROM [CharacterEquipment] "
		"WHERE "
			"[CharacterId]={} "
			"AND [EquipSlotId]={};";
	static const std::string QUERY_ALL = 
		"SELECT "
			"[EquipSlotId],"
			"[ItemId] "
		"FROM [CharacterEquipment] "
		"WHERE "
			"[CharacterId]={};";
	static const std::string DELETE_ITEM = 
		"DELETE FROM [CharacterEquipment] "
		"WHERE "
			"[CharacterId]={} "
			"AND [EquipSlotId]={};";
	static const std::string DELETE_ALL_FOR_CHARACTER = 
		"DELETE FROM [CharacterEquipment] "
		"WHERE "
			"[CharacterId]={};";
	static const std::string DELETE_ALL = 
		"DELETE FROM [CharacterEquipment];";
	static const std::string REPLACE_ITEM = 
		"REPLACE INTO [CharacterEquipment]"
		"("
			"[CharacterId],"
			"[EquipSlotId],"
			"[ItemId]"
		") "
		"VALUES({},{},{});";

	static const std::string FIELD_ITEM_ID = "ItemId";
	static const std::string FIELD_EQUIP_SLOT_ID = "EquipSlotId";

	static auto AutoCreateTable = Common::Run(CREATE_TABLE);

	std::optional<int> Equipment::Read(int characterId, int equipSlotId)
	{
		AutoCreateTable();
		auto records = Common::Execute(QUERY_ITEM, characterId, equipSlotId);
		if (!records.empty())
		{
			return common::Data::ToOptionalInt(records.front()[FIELD_ITEM_ID]);
		}
		return std::nullopt;
	}

	void Equipment::Write(int characterId, int equipSlotId, const std::optional<int>& itemId)
	{
		AutoCreateTable();
		Common::Execute(
			DELETE_ITEM, 
			characterId, 
			equipSlotId);
		if (itemId)
		{
			Common::Execute(
				REPLACE_ITEM, 
				characterId, 
				equipSlotId, 
				itemId.value());
		}
	}

	void Equipment::Clear(int characterId)
	{
		AutoCreateTable();
		Common::Execute(
			DELETE_ALL_FOR_CHARACTER,
			characterId);
	}

	std::map<int, int> Equipment::All(int characterId)
	{
		std::map<int, int> result;
		AutoCreateTable();
		auto records = Common::Execute(QUERY_ALL, characterId);
		for (auto& record : records)
		{
			result[common::Data::ToInt(record[FIELD_EQUIP_SLOT_ID])] = common::Data::ToInt(record[FIELD_ITEM_ID]);
		}
		return result;
	}

	void Equipment::Clear()
	{
		AutoCreateTable();
		Common::Execute(
			DELETE_ALL);
	}

}