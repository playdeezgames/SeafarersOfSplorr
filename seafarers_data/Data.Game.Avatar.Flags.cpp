#include <Common.Data.h>
#include "Data.Game.Avatar.Flags.h"
#include "Data.Game.Common.h"
namespace data::game::avatar
{
	const static std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [AvatarFlags]([AvatarId] INT NOT NULL,[FlagId] INT NOT NULL, UNIQUE([AvatarId],[FlagId]));";
	const static std::string QUERY_ITEM = "SELECT [FlagId] FROM [AvatarFlags] WHERE [AvatarId]={} AND [FlagId]={};";
	const static std::string REPLACE_ITEM = "REPLACE INTO [AvatarFlags]([AvatarId],[FlagId]) VALUES({},{});";
	const static std::string DELETE_ITEM = "DELETE FROM [AvatarFlags] WHERE [AvatarId]={} AND [FlagId]={};";
	const static std::string QUERY_ALL_FOR_AVATAR = "SELECT [FlagId] FROM [AvatarFlags] WHERE [AvatarId]={};";
	const static std::string FIELD_FLAG_ID = "FlagId";

	static auto AutoCreateAvatarFlagsTable = Common::Run(CREATE_TABLE);

	bool Flags::Has(int avatarId, int flagId)
	{
		AutoCreateAvatarFlagsTable();
		auto records = Common::Execute(QUERY_ITEM, avatarId, flagId);
		return !records.empty();
	}

	void Flags::Write(int avatarId, int flagId)
	{
		AutoCreateAvatarFlagsTable();
		Common::Execute(REPLACE_ITEM, avatarId, flagId);
	}

	void Flags::Clear(int avatarId, int flagId)
	{
		AutoCreateAvatarFlagsTable();
		Common::Execute(DELETE_ITEM, avatarId, flagId);
	}

	std::set<int> Flags::All(int avatarId)
	{
		AutoCreateAvatarFlagsTable();
		auto records = Common::Execute(QUERY_ALL_FOR_AVATAR, avatarId);
		std::set<int> result;
		for (auto& record : records)
		{
			result.insert(common::Data::ToInt(record[FIELD_FLAG_ID]));
		}
		return result;

	}
}