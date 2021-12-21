#include <Common.Data.h>
#include "Data.Game.Character.ItemsLegacy.h"
#include "Data.Game.Common.h"
namespace data::game::character
{
	static const std::string CREATE_TABLE = 
		"CREATE TABLE IF NOT EXISTS [CharacterItems]"
		"("
			"[CharacterId] INT NOT NULL,"
			"[ItemId] INT NOT NULL, "
			"[ItemCount] INT NOT NULL, "
			"UNIQUE([CharacterId],[ItemId])"
		");";
	static const std::string REPLACE_ITEM = 
		"REPLACE INTO [CharacterItems]"
		"("
			"[CharacterId],"
			"[ItemId],"
			"[ItemCount]"
		") "
			"VALUES({},{},{});";
	static const std::string QUERY_ITEM = 
		"SELECT "
			"[ItemCount] "
		"FROM [CharacterItems] "
		"WHERE "
			"[CharacterId]={} "
			"AND [ItemId]={};";
	static const std::string QUERY_ALL = 
		"SELECT "
			"[ItemId],"
			"[ItemCount] "
		"FROM [CharacterItems] "
		"WHERE "
			"[CharacterId]={};";
	static const std::string DELETE_ALL = 
		"DELETE FROM [CharacterItems] "
		"WHERE "
			"[CharacterId]={};";
	static const std::string DELETE_ALL_ITEMS = 
		"DELETE FROM [CharacterItems];";
	static const std::string DELETE_ITEM = 
		"DELETE FROM [CharacterItems] "
		"WHERE "
			"[CharacterId]={} "
			"AND [ItemId]={};";

	static const std::string FIELD_ITEM_ID = "ItemId";
	static const std::string FIELD_ITEM_COUNT = "ItemCount";

	static const auto AutoCreateTable = data::game::Common::Run(CREATE_TABLE);

	size_t ItemsLegacy::Read(int avatarId, const int& itemId)
	{
		AutoCreateTable();
		auto records =
			data::game::Common::Execute(QUERY_ITEM, avatarId, itemId);
		if (!records.empty())
		{
			return (size_t)common::Data::ToInt(records.front()[FIELD_ITEM_COUNT]);
		}
		return 0;
	}

	void ItemsLegacy::Write(int avatarId, const int& itemId, const size_t& count)
	{
		AutoCreateTable();
		data::game::Common::Execute(
			DELETE_ITEM, 
			avatarId, 
			itemId);
		if (count > 0)
		{
			data::game::Common::Execute(
				REPLACE_ITEM, 
				avatarId, 
				itemId, 
				count);
		}
	}

	void ItemsLegacy::Clear(int avatarId)
	{
		AutoCreateTable();
		data::game::Common::Execute(
			DELETE_ALL, 
			avatarId);
	}

	void ItemsLegacy::ClearAll()
	{
		AutoCreateTable();
		data::game::Common::Execute(DELETE_ALL_ITEMS);
	}

	std::map<int, size_t> ItemsLegacy::All(int avatarId)
	{
		std::map<int, size_t> result;
		auto records =
			data::game::Common::Execute(QUERY_ALL, avatarId);
		for (auto& record : records)
		{
			result[common::Data::ToInt(record[FIELD_ITEM_ID])] = (size_t)common::Data::ToInt(record[FIELD_ITEM_COUNT]);
		}
		return result;
	}
}