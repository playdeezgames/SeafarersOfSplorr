#include <Common.Data.h>
#include "Data.Game.Avatar.h"
#include "Data.Game.Common.h"
#include "Data.Game.Player.h"
namespace data::game
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [Players]([PlayerId] INT NOT NULL UNIQUE CHECK([PlayerId]=1),[AvatarId] INT NOT NULL UNIQUE);";
	static const std::string QUERY_ITEM = "SELECT [AvatarId] FROM [Players] WHERE [PlayerId]={};";
	static const std::string REPLACE_ITEM = "REPLACE INTO [Players]([PlayerId],[AvatarId]) VALUES ({},{});";
	static const std::string FIELD_AVATAR_ID = "AvatarId";

	static const auto AutoCreatePlayerTable = Common::Run(CREATE_TABLE);

	static const int PLAYER_ID = 1;

	int Player::GetAvatarId()
	{
		AutoCreatePlayerTable();
		auto records = Common::Execute(QUERY_ITEM, PLAYER_ID);
		if (!records.empty())
		{
			return common::Data::ToInt(records.front()[FIELD_AVATAR_ID]);
		}
		int avatarId = Avatar::NextId();
		Common::Execute(REPLACE_ITEM, PLAYER_ID, avatarId);
		return avatarId;
	}
}