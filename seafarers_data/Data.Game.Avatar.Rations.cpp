#include <Common.Data.h>
#include "Data.Game.Avatar.Rations.h"
#include "Data.Game.Common.h"
#include <format>
namespace data::game::avatar
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [AvatarRations]([AvatarId] INT NOT NULL UNIQUE,[ItemId] INT NOT NULL);";
	static auto AutoCreateAvatarRationsTable = Common::Run(CREATE_TABLE);

	static const std::string REPLACE_ITEM = "REPLACE INTO [AvatarRations]([AvatarId],[ItemId]) VALUES({},{});";
	static const std::string QUERY_ITEM = "SELECT [ItemId] FROM [AvatarRations] WHERE [AvatarId]={};";
	static const std::string DELETE_ITEM = "DELETE FROM [AvatarRations] WHERE [AvatarId]={};";
	static const std::string DELETE_ALL = "DELETE FROM [AvatarRations];";
	static const std::string FIELD_ITEM_ID = "ItemId";

	void Rations::Write(int avatarId, int itemId)
	{
		AutoCreateAvatarRationsTable();
		Common::Execute(std::format(REPLACE_ITEM, avatarId, itemId));
	}

	std::optional<int> Rations::Read(int avatarId)
	{
		AutoCreateAvatarRationsTable();
		auto records = Common::Execute(std::format(QUERY_ITEM, avatarId));
		if (!records.empty())
		{
			return common::Data::ToOptionalInt(records.front()[FIELD_ITEM_ID]);
		}
		return std::nullopt;
	}

	void Rations::Clear(int avatarId)
	{
		AutoCreateAvatarRationsTable();
		Common::Execute(std::format(DELETE_ITEM, avatarId));

	}

	void Rations::ClearAll()
	{
		AutoCreateAvatarRationsTable();
		Common::Execute(std::format(DELETE_ALL));
	}
}