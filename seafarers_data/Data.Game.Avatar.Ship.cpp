#include <Common.Data.h>
#include "Data.Game.Avatar.Ship.h"
#include "Data.Game.Common.h"
#include "Data.Game.Player.h"
#include "Data.Game.Ship.h"
#include <format>
namespace data::game::avatar
{
	static const std::string FIELD_SHIP_TYPE = "ShipType";
	static const std::string FIELD_SHIP_ID = "ShipId";
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [AvatarShips]([AvatarId] INT NOT NULL UNIQUE, [ShipType] INT NOT NULL,[ShipId] INT NOT NULL UNIQUE);";
	static const std::string REPLACE_ITEM = "REPLACE INTO [AvatarShips]([AvatarId], [ShipType],[ShipId]) VALUES({},{},{});";
	static const std::string QUERY_ITEM = "SELECT [ShipType],[ShipId] FROM [AvatarShips] WHERE [AvatarId]={};";

	static const auto AutoCreateAvatarShipTable = data::game::Common::Run(CREATE_TABLE);

	void Ship::Write(int avatarId, int shipType, int shipId)
	{
		AutoCreateAvatarShipTable();
		data::game::Common::Execute(std::format(REPLACE_ITEM, avatarId, shipType, shipId));
	}

	int Ship::ReadShipType(int avatarId)
	{
		AutoCreateAvatarShipTable();
		auto records = data::game::Common::Execute(std::format(QUERY_ITEM, avatarId));
		return common::Data::ToInt(records.front()[FIELD_SHIP_TYPE]);
	}

	int Ship::ReadShipId(int avatarId)
	{
		AutoCreateAvatarShipTable();
		auto records = data::game::Common::Execute(std::format(QUERY_ITEM, avatarId));
		return common::Data::ToInt(records.front()[FIELD_SHIP_ID]);
	}
}