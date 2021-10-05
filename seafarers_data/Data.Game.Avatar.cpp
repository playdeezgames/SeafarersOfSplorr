#include <Common.Data.h>
#include <Common.Utility.h>
#include "Data.Game.Avatar.h"
#include "Data.Game.Common.h"
#include "Data.Game.Player.h"
#include <format>
#include <optional>
namespace data::game
{
	static const std::string FIELD_X = "X";
	static const std::string FIELD_Y = "Y";
	static const std::string FIELD_STATE = "State";
	static const std::string FIELD_NAME = "Name";
	static const std::string FIELD_MAX_AVATAR_ID = "MaxAvatarId";
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [Avatars]([AvatarId] INT NOT NULL UNIQUE,[X] REAL NOT NULL,[Y] REAL NOT NULL,[State] INT NOT NULL, [Name] TEXT NOT NULL);";
	static const std::string QUERY_ITEM= "SELECT [X], [Y], [State],[Name] FROM [Avatars] WHERE [AvatarId] = {};";
	static const std::string REPLACE_ITEM = "REPLACE INTO [Avatars]([AvatarId],[X],[Y],[State],[Name]) VALUES ({},{},{},{},{});";
	static const std::string QUERY_MAX_AVATAR_ID = "SELECT COALESCE(MAX([AvatarId]),0) [MaxAvatarId] FROM [Avatars];";

	static const auto AutoCreateAvatarTable = data::game::Common::Run(CREATE_TABLE);

	std::optional<Avatar> Avatar::Read(int avatarId)
	{
		AutoCreateAvatarTable();
		auto result = data::game::Common::Execute(
			std::format(
				QUERY_ITEM,
				avatarId));
		if (!result.empty())
		{
			const auto& record = result.front();
			Avatar data =
			{
				{
					common::Data::ToDouble(record.find(FIELD_X)->second),
					common::Data::ToDouble(record.find(FIELD_Y)->second)
				},
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
			std::format(
				REPLACE_ITEM,
				avatarId,
				avatarData.location.GetX(),
				avatarData.location.GetY(),
				avatarData.state,
				common::Data::QuoteString(avatarData.name)));
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

	void Avatar::Write(const Avatar& avatar)
	{
		Write(Player::GetAvatarId(), avatar);
	}

	std::optional<Avatar> Avatar::Read()
	{
		return Read(Player::GetAvatarId());
	}

	void Avatar::WriteState(int state)
	{
		WriteState(Player::GetAvatarId(), state);
	}

	std::optional<int> Avatar::ReadState()
	{
		return ReadState(Player::GetAvatarId());
	}

	int Avatar::NextId()
	{
		AutoCreateAvatarTable();
		return common::Data::ToInt(Common::Execute(QUERY_MAX_AVATAR_ID).front()[FIELD_MAX_AVATAR_ID])+1;
	}

}