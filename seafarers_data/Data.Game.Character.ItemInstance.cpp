#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Character.ItemInstance.h"
namespace data::game::character
{
	static const std::string CREATE_TABLE = 
		"CREATE TABLE IF NOT EXISTS [CharacterItemInstances]"
		"("
			"[ItemInstanceId] INT NOT NULL UNIQUE,"
			"[CharacterId] INT NOT NULL"
		");";
	static const std::string REPLACE_ITEM = 
		"REPLACE INTO [CharacterItemInstances]"
		"("
			"[ItemInstanceId],"
			"[CharacterId]"
		") "
		"VALUES({},{});";
	static const std::string PURGE_ITEMS = 
		"DELETE FROM [CharacterItemInstances] cii "
		"LEFT JOIN [ItemInstances] ii "
			"ON ii.[ItemInstanceId]=cii.[ItemInstanceId] "
		"WHERE "
			"ii.[ItemInstanceId] IS NULL;";
	static const std::string QUERY_ITEMS_FOR_CHARACTER = 
		"SELECT "
			"[ItemInstanceId] "
		"FROM [CharacterItemInstances] "
		"WHERE "
			"[CharacterId]={};";
	static const std::string QUERY_ITEMS_FOR_ITEM_INSTANCE = 
		"SELECT "
			"[CharacterId] "
		"FROM [CharacterItemInstances] "
		"WHERE "
			"[ItemInstanceId]={};";

	static const std::string FIELD_CHARACTER_ID = "CharacterId";
	static const std::string FIELD_ITEM_INSTANCE_ID = "ItemInstanceId";

	static const auto AutoCreateTable = Common::Run(CREATE_TABLE);

	void ItemInstance::Write(int itemInstanceId, int characterId)
	{
		AutoCreateTable();
		Common::Execute(REPLACE_ITEM, itemInstanceId, characterId);
	}

	void ItemInstance::Purge()
	{
		AutoCreateTable();
		Common::Execute(PURGE_ITEMS);
	}

	std::list<int> ItemInstance::ReadForCharacter(int characterId)
	{
		AutoCreateTable();
		auto records = Common::Execute(QUERY_ITEMS_FOR_CHARACTER, characterId);
		std::list<int> result;
		for (auto record : records)
		{
			result.push_back(common::Data::ToInt(record[FIELD_ITEM_INSTANCE_ID]));
		}
		return result;
	}

	std::optional<int> ItemInstance::ReadForItemInstance(int itemInstanceId)
	{
		AutoCreateTable();
		auto records = Common::Execute(QUERY_ITEMS_FOR_ITEM_INSTANCE, itemInstanceId);
		if (!records.empty())
		{
			return common::Data::ToOptionalInt(records.front()[FIELD_CHARACTER_ID]);
		}
		return std::nullopt;
	}
}