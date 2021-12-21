#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Ship.h"
namespace data::game
{
	static const std::string CREATE_TABLE = 
		"CREATE TABLE IF NOT EXISTS [Ships]"
		"("
			"[ShipId] INTEGER PRIMARY KEY AUTOINCREMENT,"
			"[ShipType] INT NOT NULL,"
			"[Name] TEXT NOT NULL,"
			"[X] REAL NOT NULL,"
			"[Y] REAL NOT NULL,"
			"[Heading] REAL NOT NULL,"
			"[Speed] REAL NOT NULL"
		");";
	static const std::string QUERY_ITEM = 
		"SELECT "
			"[ShipId],"
			"[ShipType],"
			"[Name],"
			"[X],"
			"[Y],"
			"[Heading],"
			"[Speed] "
		"FROM [Ships] "
		"WHERE "
			"[ShipId]={};";
	static const std::string INSERT_ITEM = 
		"INSERT INTO [Ships] "
		"("
			"[ShipType],"
			"[Name],"
			"[X],"
			"[Y],"
			"[Heading],"
			"[Speed]"
		") "
		"VALUES({},{},{:.4f},{:.4f},{:.4f},{:.4f});";
	static const std::string UPDATE_ITEM = 
		"UPDATE [Ships] "
		"SET "
			"[ShipType]={},"
			"[Name]={},"
			"[X]={:4f},"
			"[Y]={:4f},"
			"[Heading]={},"
			"[Speed]={} "
		"WHERE "
			"[ShipId]={};";
	static const std::string DELETE_ALL = 
		"DELETE FROM [Ships];";
	static const std::string QUERY_ALL = 
		"SELECT "
			"[ShipId] "
		"FROM [Ships];";

	static const std::string FIELD_SHIP_ID = "ShipId";
	static const std::string FIELD_SHIP_TYPE = "ShipType";
	static const std::string FIELD_NAME = "Name";
	static const std::string FIELD_X = "X";
	static const std::string FIELD_Y = "Y";
	static const std::string FIELD_HEADING = "Heading";
	static const std::string FIELD_SPEED = "Speed";

	static const auto AutoCreateShipTable = Common::Run(CREATE_TABLE);

	int Ship::Write(const Ship& ship)
	{
		AutoCreateShipTable();
		if (ship.shipId == 0)
		{
			Common::Execute(
				INSERT_ITEM,
				ship.shipType,
				common::Data::QuoteString(ship.name),
				ship.location.GetX(),
				ship.location.GetY(),
				ship.heading,
				ship.speed);
			return Common::LastInsertedIndex();
		}
		else
		{
			Common::Execute(
				UPDATE_ITEM,
				ship.shipType,
				common::Data::QuoteString(ship.name),
				ship.location.GetX(),
				ship.location.GetY(),
				ship.heading,
				ship.speed,
				ship.shipId);
			return ship.shipId;
		}
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

	void Ship::Clear()
	{
		AutoCreateShipTable();
		Common::Execute(DELETE_ALL);
	}

	std::list<int> Ship::All()
	{
		AutoCreateShipTable();
		std::list<int> result;
		auto records = Common::Execute(QUERY_ALL);
		for (auto& record : records)
		{
			result.push_back(common::Data::ToInt(record[FIELD_SHIP_ID]));
		}
		return result;
	}
}