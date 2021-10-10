#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Ship.h"
namespace data::game//20211010
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [Ships]([ShipId] INT NOT NULL UNIQUE,[ShipType] INT NOT NULL,[Name] TEXT NOT NULL,[X] REAL NOT NULL,[Y] REAL NOT NULL,[Heading] REAL NOT NULL,[Speed] REAL NOT NULL);";
	static const std::string QUERY_ITEM = "SELECT [ShipId],[ShipType],[Name],[X],[Y],[Heading],[Speed] FROM [Ships] WHERE [ShipId]={};";
	static const std::string REPLACE_ITEM = "REPLACE INTO [Ships] ([ShipId],[ShipType],[Name],[X],[Y],[Heading],[Speed]) VALUES({},{},{},{:.4f},{:.4f},{:.4f},{:.4f});";
	static const std::string QUERY_MAX_ID = "SELECT COALESCE(MAX([ShipId]),0) MaxShipId FROM [Ships];";
	static const std::string DELETE_ALL = "DELETE FROM [Ships];";

	static const std::string FIELD_SHIP_ID = "ShipId";
	static const std::string FIELD_MAX_SHIP_ID = "MaxShipId";
	static const std::string FIELD_SHIP_TYPE = "ShipType";
	static const std::string FIELD_NAME = "Name";
	static const std::string FIELD_X = "X";
	static const std::string FIELD_Y = "Y";
	static const std::string FIELD_HEADING = "Heading";
	static const std::string FIELD_SPEED = "Speed";

	static const auto AutoCreateShipTable = Common::Run(CREATE_TABLE);

	void Ship::Write(const Ship& ship)
	{
		AutoCreateShipTable();
		Common::Execute(
			REPLACE_ITEM,
			ship.shipId,
			ship.shipType,
			common::Data::QuoteString(ship.name),
			ship.location.GetX(),
			ship.location.GetY(),
			ship.heading,
			ship.speed);
	}

	static Ship ToShip(const std::map<std::string, std::string> table)
	{
		return 
		{
			common::Data::ToInt(table.find(FIELD_SHIP_ID)->second),
			common::Data::ToInt(table.find(FIELD_SHIP_TYPE)->second),
			table.find(FIELD_NAME)->second,
			{
				common::Data::ToDouble(table.find(FIELD_X)->second),
				common::Data::ToDouble(table.find(FIELD_Y)->second)
			},
			common::Data::ToDouble(table.find(FIELD_HEADING)->second),
			common::Data::ToDouble(table.find(FIELD_SPEED)->second)
		};
	}

	std::optional<Ship> Ship::Read(int shipId)
	{
		AutoCreateShipTable();
		auto records = Common::Execute(QUERY_ITEM, shipId);
		if (!records.empty())
		{
			return ToShip(records.front());
		}
		return std::nullopt;
	}

	int Ship::NextId()
	{
		AutoCreateShipTable();
		return common::Data::ToInt(
			Common::Execute(QUERY_MAX_ID)
			.front()[FIELD_MAX_SHIP_ID]) + 1;
	}

	void Ship::Clear()
	{
		AutoCreateShipTable();
		Common::Execute(DELETE_ALL);
	}
}