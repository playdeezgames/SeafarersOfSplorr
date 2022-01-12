#include <Common.Data.h>
#include "Data.Game.Character.h"
#include "Data.Game.Common.h"
#include "Data.Game.Player.h"
namespace data::game
{
	using namespace std::string_view_literals;
	static constexpr std::string_view CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [Players]
		(
			[PlayerId] INT NOT NULL UNIQUE CHECK([PlayerId]=1),
			[CharacterId] INT NOT NULL UNIQUE
		);)"sv;
	static constexpr std::string_view QUERY_ITEM =
		R"(SELECT 
			[CharacterId] 
		FROM [Players] 
		WHERE 
			[PlayerId]={};)"sv;
	static constexpr std::string_view REPLACE_ITEM =
		R"(REPLACE INTO [Players]
		(
			[PlayerId],
			[CharacterId]
		) 
		VALUES ({},{});)"sv;

	static constexpr std::string_view FIELD_CHARACTER_ID = "CharacterId";

	static const int PLAYER_ID = 1;

	void Player::Initialize()
	{
		Character::Initialize();
		Common::Execute(CREATE_TABLE);
	}

	void Player::Create(int avatarId)
	{
		Initialize();
		Common::Execute(REPLACE_ITEM, PLAYER_ID, avatarId);
	}

	std::optional<int> Player::GetCharacterId()
	{
		Initialize();
		return Common::TryToInt(
			Common::TryExecuteForOne(QUERY_ITEM, PLAYER_ID),
			FIELD_CHARACTER_ID);
	}
}