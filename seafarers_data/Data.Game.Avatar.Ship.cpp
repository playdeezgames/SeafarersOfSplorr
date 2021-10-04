#include <Common.Data.h>
#include "Data.Game.Avatar.Ship.h"
#include "Data.Game.Common.h"
#include "Data.Game.Player.h"
#include <format>
namespace data::game::avatar
{
	static const std::string FIELD_SHIP_ID = "ShipType";
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [AvatarShips]([AvatarId] INT NOT NULL UNIQUE, [ShipType] INT NOT NULL);";
	static const std::string REPLACE_ITEM = "REPLACE INTO [AvatarShips]([AvatarId], [ShipType]) VALUES({},{});";
	static const std::string QUERY_ITEM = "SELECT [ShipType] FROM [AvatarShips] WHERE [AvatarId]={};";

	static const auto AutoCreateAvatarShipTable = data::game::Common::Run(CREATE_TABLE);

	void Ship::Write(int avatarId, int ship)
	{
		AutoCreateAvatarShipTable();
		data::game::Common::Execute(std::format(REPLACE_ITEM, avatarId, ship));
	}

	int Ship::Read(int avatarId)
	{
		AutoCreateAvatarShipTable();
		auto records = data::game::Common::Execute(std::format(QUERY_ITEM, avatarId));
		return common::Data::ToInt(records.front()[FIELD_SHIP_ID]);
	}
}