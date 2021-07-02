#include "Data.Game.Avatar.h"
#include "Data.SQLite.Stores.h"
#include <optional>
#include <format>
#include "Common.Utility.h"
namespace data::game::Avatar
{
	static void AutoCreateAvatarTable()
	{
		data::sqlite::Stores::Execute(data::sqlite::Store::IN_MEMORY, "CREATE TABLE IF NOT EXISTS [Avatars]([AvatarId] INT NOT NULL UNIQUE,[X] REAL NOT NULL,[Y] REAL NOT NULL,[Direction] REAL NOT NULL,[Speed] REAL NOT NULL);");
	}

	const int AVATAR_ID = 1;
	const std::string FIELD_X = "X";
	const std::string FIELD_Y = "Y";
	const std::string FIELD_DIRECTION = "Direction";
	const std::string FIELD_SPEED = "Speed";

	std::optional<AvatarData> Read()
	{
		AutoCreateAvatarTable();
		auto query = 
			std::format(
				"SELECT [X], [Y], [Direction], [Speed] FROM [Avatars] WHERE [AvatarId] = {};", 
				AVATAR_ID);
		auto result = data::sqlite::Stores::Execute(data::sqlite::Store::IN_MEMORY, query);
		if (!result.empty())
		{
			const auto& record = result.front();
			AvatarData data =
			{
				{
					common::Utility::StringToDouble(record.find(FIELD_X)->second),
					common::Utility::StringToDouble(record.find(FIELD_Y)->second)
				},
				common::Utility::StringToDouble(record.find(FIELD_DIRECTION)->second),
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
				"REPLACE INTO [Avatars]([AvatarId],[X],[Y],[Direction],[Speed]) VALUES ({},{},{},{},{});", 
				AVATAR_ID, 
				avatarData.location.GetX(), 
				avatarData.location.GetY(), 
				avatarData.direction, 
				avatarData.speed);
		data::sqlite::Stores::Execute(data::sqlite::Store::IN_MEMORY, query);
	}
}