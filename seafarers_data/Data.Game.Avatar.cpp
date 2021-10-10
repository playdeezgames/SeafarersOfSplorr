#include <Common.Data.h>
#include <Common.Utility.h>
#include "Data.Game.Avatar.h"
#include "Data.Game.Common.h"
#include "Data.Game.Player.h"
#include <optional>
namespace data::game//20211010
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [Avatars]([AvatarId] INT NOT NULL UNIQUE,[State] INT NOT NULL, [Name] TEXT NOT NULL);";
	static const std::string QUERY_ITEM= "SELECT [State],[Name] FROM [Avatars] WHERE [AvatarId] = {};";
	static const std::string REPLACE_ITEM = "REPLACE INTO [Avatars]([AvatarId],[State],[Name]) VALUES ({},{},{});";
	static const std::string QUERY_MAX_AVATAR_ID = "SELECT COALESCE(MAX([AvatarId]),0) [MaxAvatarId] FROM [Avatars];";

	static const std::string FIELD_STATE = "State";
	static const std::string FIELD_NAME = "Name";
	static const std::string FIELD_MAX_AVATAR_ID = "MaxAvatarId";

	static const auto AutoCreateAvatarTable = data::game::Common::Run(CREATE_TABLE);

	std::optional<Avatar> Avatar::Read(int avatarId)
	{
		AutoCreateAvatarTable();
		auto result = data::game::Common::Execute(
				QUERY_ITEM,
				avatarId);
		if (!result.empty())
		{
			const auto& record = result.front();
			Avatar data =
			{
				common::Data::ToInt(record.find(FIELD_STATE)->second),
				record.find(FIELD_NAME)->second
			};
			return data;
		}
		return std::nullopt;
	}

	void Avatar::Write(int avatarId, const Avatar& avatarData)
	{
		AutoCreateAvatarTable();
		data::game::Common::Execute(
				REPLACE_ITEM,
				avatarId,
				avatarData.state,
				common::Data::QuoteString(avatarData.name));
	}

	void Avatar::WriteState(int avatarId, int state)
	{
		auto data = Read(avatarId);
		if (data)
		{
			data.value().state = state;
			Write(avatarId,data.value());
		}
	}

	std::optional<int> Avatar::ReadState(int avatarId)
	{
		auto data = Read(avatarId);
		if (data)
		{
			return data.value().state;
		}
		return std::nullopt;
	}

	int Avatar::NextId()
	{
		AutoCreateAvatarTable();
		return common::Data::ToInt(Common::Execute(QUERY_MAX_AVATAR_ID).front()[FIELD_MAX_AVATAR_ID])+1;
	}
}