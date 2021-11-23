#include <Common.Data.h>
#include "Data.Game.Character.h"
#include "Data.Game.Common.h"
#include "Data.Game.Player.h"
namespace data::game
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [Players]([PlayerId] INT NOT NULL UNIQUE CHECK([PlayerId]=1),[CharacterId] INT NOT NULL UNIQUE);";
	static const std::string QUERY_ITEM = "SELECT [CharacterId] FROM [Players] WHERE [PlayerId]={};";
	static const std::string REPLACE_ITEM = "REPLACE INTO [Players]([PlayerId],[CharacterId]) VALUES ({},{});";

	static const std::string FIELD_AVATAR_ID = "CharacterId";

	static const auto AutoCreatePlayerTable = Common::Run(CREATE_TABLE);

	static const int PLAYER_ID = 1;

	static int CreatePlayer()
	{
		AutoCreatePlayerTable();
		int avatarId = Character::NextId();
		Common::Execute(REPLACE_ITEM, PLAYER_ID, avatarId);
		return avatarId;
	}

	int Player::GetAvatarId()
	{
		AutoCreatePlayerTable();
		auto records = Common::Execute(QUERY_ITEM, PLAYER_ID);
		if (!records.empty())
		{
			return common::Data::ToInt(records.front()[FIELD_AVATAR_ID]);
		}
		return CreatePlayer();
	}
}