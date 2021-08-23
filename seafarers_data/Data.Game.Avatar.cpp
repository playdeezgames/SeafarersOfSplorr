#include <Common.Data.h>
#include <Common.Utility.h>
#include "Data.Game.Avatar.h"
#include "Data.Game.Common.h"
#include <format>
#include <optional>
namespace data::game::Avatar
{
	const std::string FIELD_X = "X";
	const std::string FIELD_Y = "Y";
	const std::string FIELD_HEADING = "Heading";
	const std::string FIELD_SPEED = "Speed";
	const std::string FIELD_STATE = "State";
	const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [Avatars]([AvatarId] INT NOT NULL UNIQUE,[X] REAL NOT NULL,[Y] REAL NOT NULL,[Heading] REAL NOT NULL,[Speed] REAL NOT NULL, [State] INT NOT NULL);";
	const std::string QUERY_ITEM= "SELECT [X], [Y], [Heading], [Speed], [State] FROM [Avatars] WHERE [AvatarId] = {};";
	const std::string REPLACE_ITEM = "REPLACE INTO [Avatars]([AvatarId],[X],[Y],[Heading],[Speed],[State]) VALUES ({},{},{},{},{},{});";

	const auto AutoCreateAvatarTable = data::game::Common::Run(CREATE_TABLE);

	std::optional<AvatarData> Read()
	{
		AutoCreateAvatarTable();
		auto result = data::game::Common::Execute(
			std::format(
				QUERY_ITEM,
				data::game::Common::AVATAR_ID));
		if (!result.empty())
		{
			const auto& record = result.front();
			AvatarData data =
			{
				{
					common::Data::ToDouble(record.find(FIELD_X)->second),
					common::Data::ToDouble(record.find(FIELD_Y)->second)
				},
				common::Data::ToDouble(record.find(FIELD_HEADING)->second),
				common::Data::ToDouble(record.find(FIELD_SPEED)->second),
				common::Data::ToInt(record.find(FIELD_STATE)->second)
			};
			return data;
		}
		return std::nullopt;
	}

	void Write(const AvatarData& avatarData)
	{
		AutoCreateAvatarTable();
		data::game::Common::Execute(
			std::format(
				REPLACE_ITEM,
				data::game::Common::AVATAR_ID,
				avatarData.location.GetX(),
				avatarData.location.GetY(),
				avatarData.heading,
				avatarData.speed,
				avatarData.state));
	}

	void SetState(int state)
	{
		AutoCreateAvatarTable();
		auto data = Read();
		if (data)
		{
			data.value().state = state;
			Write(data.value());
		}
	}

	std::optional<int> GetState()
	{
		AutoCreateAvatarTable();
		auto data = Read();
		if (data)
		{
			return data.value().state;
		}
		return std::nullopt;
	}
}