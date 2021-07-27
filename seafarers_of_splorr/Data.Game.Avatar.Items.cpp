#include "Common.Data.h"
#include "Data.Game.Avatar.Items.h"
#include "Data.Game.Common.h"
#include <format>
namespace data::game::avatar::Items
{
	const std::string FIELD_ITEM_ID = "ItemId";
	const std::string FIELD_ITEM_COUNT = "ItemCount";
	const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [AvatarItems]([AvatarId] INT NOT NULL,[ItemId] INT NOT NULL, [ItemCount] INT NOT NULL, UNIQUE([AvatarId],[ItemId]))";
	const std::string REPLACE_ITEM = "REPLACE INTO [AvatarItems]([AvatarId],[ItemId],[ItemCount]) VALUES({},{},{});";
	const std::string QUERY_ITEM = "SELECT [ItemCount] FROM [AvatarItems] WHERE [AvatarId]={} AND [ItemId]={};";
	const std::string QUERY_ALL = "SELECT [ItemId],[ItemCount] FROM [AvatarItems] WHERE [AvatarId]={};";
	const std::string DELETE_ALL = "DELETE FROM [AvatarItems];";
	const std::string DELETE_ITEM = "DELETE FROM [AvatarItems] WHERE [AvatarId]={} AND [ItemId]={};";

	static void AutoCreateAvatarItemsTable()
	{
		data::game::Common::Execute(CREATE_TABLE);
	}

	size_t Read(const int& itemId)
	{
		AutoCreateAvatarItemsTable();
		auto records =
			data::game::Common::Execute(std::format(QUERY_ITEM,data::game::Common::AVATAR_ID, itemId));
		if (!records.empty())
		{
			auto& record = records.front();
			return (size_t)common::Data::StringToInt(record[FIELD_ITEM_COUNT]);
		}
		return 0;
	}

	void Write(const int& itemId, const size_t& count)
	{
		AutoCreateAvatarItemsTable();
		data::game::Common::Execute(std::format(DELETE_ITEM, data::game::Common::AVATAR_ID, itemId));
		if (count > 0)
		{
			data::game::Common::Execute(std::format(REPLACE_ITEM, data::game::Common::AVATAR_ID, itemId, count));
		}
	}

	void Clear()
	{
		AutoCreateAvatarItemsTable();
		data::game::Common::Execute(DELETE_ALL);
	}

	std::map<int, size_t> ReadAll()
	{
		std::map<int, size_t> result;
		auto records =
			data::game::Common::Execute(std::format(QUERY_ALL, data::game::Common::AVATAR_ID));
		for (auto& record : records)
		{
			result[common::Data::StringToInt(record[FIELD_ITEM_ID])] = (size_t)common::Data::StringToInt(record[FIELD_ITEM_COUNT]);
		}
		return result;
	}

}