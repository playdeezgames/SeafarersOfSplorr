#include <Common.Data.h>
#include <Common.Utility.h>
#include "Data.Game.Avatar.h"
#include "Data.Game.Common.h"
#include "Data.Game.Player.h"
#include <optional>
namespace data::game//20211011
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [Avatars]([AvatarId] INT NOT NULL UNIQUE,[State] INT NOT NULL, [Name] TEXT NOT NULL);";
	static const std::string QUERY_ITEM= "SELECT [State],[Name] FROM [Avatars] WHERE [AvatarId] = {};";
	static const std::string REPLACE_ITEM = "REPLACE INTO [Avatars]([AvatarId],[State],[Name]) VALUES ({},{},{});";
	static const std::string QUERY_MAX_AVATAR_ID = "SELECT COALESCE(MAX([AvatarId]),0) [MaxAvatarId] FROM [Avatars];";
	static const std::string QUERY_ALL = "SELECT [AvatarId] FROM [Avatars];";

	static const std::string FIELD_STATE = "State";
	static const std::string FIELD_NAME = "Name";
	static const std::string FIELD_MAX_AVATAR_ID = "MaxAvatarId";
	static const std::string FIELD_AVATAR_ID = "AvatarId";

	static const auto AutoCreateAvatarTable = data::game::Common::Run(CREATE_TABLE);

	static Avatar ToAvatar(const std::map<std::string, std::string> record)
	{
		return
		{
			common::Data::ToInt(record.find(FIELD_STATE)->second),
			record.find(FIELD_NAME)->second
		};
	}

	std::optional<Avatar> Avatar::Read(int avatarId)
	{
		AutoCreateAvatarTable();
		auto records = Common::Execute(
				QUERY_ITEM,
				avatarId);
		if (!records.empty())
		{
			return ToAvatar(records.front());
		}
		return std::nullopt;
	}

	void Avatar::Write(int avatarId, const Avatar& avatar)
	{
		AutoCreateAvatarTable();
		Common::Execute(
			REPLACE_ITEM,
			avatarId,
			avatar.state,
			common::Data::QuoteString(avatar.name));
	}

	int Avatar::NextId()
	{
		AutoCreateAvatarTable();
		return common::Data::ToInt(Common::Execute(QUERY_MAX_AVATAR_ID).front()[FIELD_MAX_AVATAR_ID])+1;
	}

	std::list<int> Avatar::All()
	{
		AutoCreateAvatarTable();
		auto records = Common::Execute(QUERY_ALL);
		std::list<int> result;
		for (auto& record : records)
		{
			result.push_back(common::Data::ToInt(record[FIELD_AVATAR_ID]));
		}
		return result;
	}

}