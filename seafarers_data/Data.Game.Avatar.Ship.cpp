#include <Common.Data.h>
#include "Data.Game.Avatar.Ship.h"
#include "Data.Game.Common.h"
#include "Data.Game.Player.h"
#include "Data.Game.Ship.h"
namespace data::game::avatar//20211011
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [AvatarShips]([AvatarId] INT NOT NULL UNIQUE, [ShipId] INT NOT NULL,[BerthType] INT NOT NULL);";
	static const std::string REPLACE_ITEM = "REPLACE INTO [AvatarShips]([AvatarId], [ShipId], [BerthType]) VALUES({},{},{});";
	static const std::string QUERY_ITEM = "SELECT [ShipId],[BerthType] FROM [AvatarShips] WHERE [AvatarId]={};";
	static const std::string QUERY_CREW_FOR_SHIP = "SELECT [AvatarId], [BerthType] FROM [AvatarShips] WHERE [ShipId]={};";

	static const std::string FIELD_SHIP_ID = "ShipId";
	static const std::string FIELD_BERTH_TYPE = "BerthType";
	static const std::string FIELD_AVATAR_ID = "AvatarId";

	void AutoCreateAvatarShipTable()
	{
		data::game::Common::Execute(CREATE_TABLE);
	}

	void Ship::Write(int avatarId, const Ship& ship)
	{
		AutoCreateAvatarShipTable();
		data::game::Common::Execute(
			REPLACE_ITEM, 
			avatarId, 
			ship.shipId, 
			ship.berthType);
	}

	static Ship ToShip(const std::map<std::string, std::string>& record)
	{
		return
		{
			common::Data::ToInt(record.find(FIELD_SHIP_ID)->second),
			common::Data::ToInt(record.find(FIELD_BERTH_TYPE)->second)
		};
	}

	std::optional<Ship> Ship::Read(int avatarId)
	{
		AutoCreateAvatarShipTable();
		auto records = data::game::Common::Execute(QUERY_ITEM, avatarId);
		if (!records.empty())
		{
			return ToShip(records.front());
		}
		return std::nullopt;
	}
}