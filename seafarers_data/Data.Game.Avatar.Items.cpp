#include <Common.Data.h>
#include "Data.Game.Avatar.Items.h"
#include "Data.Game.Common.h"
#include <format>
namespace data::game::avatar
{
	static const std::string FIELD_ITEM_ID = "ItemId";
	static const std::string FIELD_ITEM_COUNT = "ItemCount";
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [AvatarItems]([AvatarId] INT NOT NULL,[ItemId] INT NOT NULL, [ItemCount] INT NOT NULL, UNIQUE([AvatarId],[ItemId]))";
	static const std::string REPLACE_ITEM = "REPLACE INTO [AvatarItems]([AvatarId],[ItemId],[ItemCount]) VALUES({},{},{});";
	static const std::string QUERY_ITEM = "SELECT [ItemCount] FROM [AvatarItems] WHERE [AvatarId]={} AND [ItemId]={};";
	static const std::string QUERY_ALL = "SELECT [ItemId],[ItemCount] FROM [AvatarItems] WHERE [AvatarId]={};";
	static const std::string DELETE_ALL = "DELETE FROM [AvatarItems] WHERE [AvatarId]={};";
	static const std::string DELETE_ALL_ITEMS = "DELETE FROM [AvatarItems];";
	static const std::string DELETE_ITEM = "DELETE FROM [AvatarItems] WHERE [AvatarId]={} AND [ItemId]={};";

	static const auto AutoCreateAvatarItemsTable = data::game::Common::Run(CREATE_TABLE);

	size_t Items::Read(int avatarId, const int& itemId)
	{
		AutoCreateAvatarItemsTable();
		auto records =
			data::game::Common::Execute(std::format(QUERY_ITEM, avatarId, itemId));
		if (!records.empty())
		{
			auto& record = records.front();
			return (size_t)common::Data::ToInt(record[FIELD_ITEM_COUNT]);
		}
		return 0;
	}

	void Items::Write(int avatarId, const int& itemId, const size_t& count)
	{
		AutoCreateAvatarItemsTable();
		data::game::Common::Execute(std::format(DELETE_ITEM, avatarId, itemId));
		if (count > 0)
		{
			data::game::Common::Execute(std::format(REPLACE_ITEM, avatarId, itemId, count));
		}
	}

	void Items::Clear(int avatarId)
	{
		AutoCreateAvatarItemsTable();
		data::game::Common::Execute(std::format(DELETE_ALL, avatarId));
	}

	void Items::ClearAll()
	{
		AutoCreateAvatarItemsTable();
		data::game::Common::Execute(DELETE_ALL_ITEMS);
	}

	std::map<int, size_t> Items::All(int avatarId)
	{
		std::map<int, size_t> result;
		auto records =
			data::game::Common::Execute(std::format(QUERY_ALL, avatarId));
		for (auto& record : records)
		{
			result[common::Data::ToInt(record[FIELD_ITEM_ID])] = (size_t)common::Data::ToInt(record[FIELD_ITEM_COUNT]);
		}
		return result;
	}

	size_t Items::Read(const int& itemId)
	{
		return Read(Common::AVATAR_ID, itemId);
	}

	std::map<int, size_t> Items::All()
	{
		return All(Common::AVATAR_ID);
	}

	void Items::Write(const int& itemId, const size_t& itemCount)
	{
		Write(Common::AVATAR_ID, itemId, itemCount);
	}

	void Items::Clear()
	{
		Clear(Common::AVATAR_ID);
	}
}