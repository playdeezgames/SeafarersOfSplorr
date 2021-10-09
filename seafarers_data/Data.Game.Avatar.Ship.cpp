#include <Common.Data.h>
#include "Data.Game.Avatar.Ship.h"
#include "Data.Game.Common.h"
#include "Data.Game.Player.h"
#include "Data.Game.Ship.h"
namespace data::game::avatar
{
	static const std::string FIELD_SHIP_ID = "ShipId";
	static const std::string FIELD_BERTH_TYPE = "BerthType";
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [AvatarShips]([AvatarId] INT NOT NULL UNIQUE, [ShipId] INT NOT NULL,[BerthType] INT NOT NULL);";
	static const std::string REPLACE_ITEM = "REPLACE INTO [AvatarShips]([AvatarId], [ShipId], [BerthType]) VALUES({},{},{});";
	static const std::string QUERY_ITEM = "SELECT [ShipId],[BerthType] FROM [AvatarShips] WHERE [AvatarId]={};";

	static const auto AutoCreateAvatarShipTable = data::game::Common::Run(CREATE_TABLE);

	void Ship::Write(int avatarId, const Ship& ship)
	{
		AutoCreateAvatarShipTable();
		data::game::Common::Execute(REPLACE_ITEM, avatarId, ship.shipId, ship.berthType);
	}

	std::optional<Ship> Ship::Read(int avatarId)
	{
		AutoCreateAvatarShipTable();
		auto records = data::game::Common::Execute(QUERY_ITEM, avatarId);
		if (!records.empty())
		{
			Ship ship = 
			{ 
				common::Data::ToInt(records.front()[FIELD_SHIP_ID]),
				common::Data::ToInt(records.front()[FIELD_BERTH_TYPE])
			};
			return ship;
		}
		return std::nullopt;
	}
}