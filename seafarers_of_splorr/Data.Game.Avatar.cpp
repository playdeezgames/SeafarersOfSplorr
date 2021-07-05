#include "Data.Game.Avatar.h"
#include <optional>
#include <format>
#include "Common.Utility.h"
#include "Data.Game.Common.h"
namespace data::game::Avatar
{
	const int AVATAR_ID = 1;
	const std::string FIELD_X = "X";
	const std::string FIELD_Y = "Y";
	const std::string FIELD_HEADING = "Heading";
	const std::string FIELD_SPEED = "Speed";
	const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [Avatars]([AvatarId] INT NOT NULL UNIQUE,[X] REAL NOT NULL,[Y] REAL NOT NULL,[Heading] REAL NOT NULL,[Speed] REAL NOT NULL);";
	const std::string QUERY_ITEM= "SELECT [X], [Y], [Heading], [Speed] FROM [Avatars] WHERE [AvatarId] = {};";
	const std::string REPLACE_ITEM = "REPLACE INTO [Avatars]([AvatarId],[X],[Y],[Heading],[Speed]) VALUES ({},{},{},{},{});";

	static void AutoCreateAvatarTable()
	{
		data::game::Common::Execute(CREATE_TABLE);
	}

	std::optional<AvatarData> Read()
	{
		AutoCreateAvatarTable();
		auto query = 
			std::format(
				QUERY_ITEM,
				AVATAR_ID);
		auto result = data::game::Common::Execute(query);
		if (!result.empty())
		{
			const auto& record = result.front();
			AvatarData data =
			{
				{
					common::Utility::StringToDouble(record.find(FIELD_X)->second),
					common::Utility::StringToDouble(record.find(FIELD_Y)->second)
				},
				common::Utility::StringToDouble(record.find(FIELD_HEADING)->second),
				common::Utility::StringToDouble(record.find(FIELD_SPEED)->second)
			};
			return data;
		}
		return std::nullopt;
	}

	void Write(const AvatarData& avatarData)
	{
		AutoCreateAvatarTable();
		auto query = 
			std::format(
				REPLACE_ITEM, 
				AVATAR_ID, 
				avatarData.location.GetX(), 
				avatarData.location.GetY(), 
				avatarData.heading, 
				avatarData.speed);
		data::game::Common::Execute(query);
	}
}