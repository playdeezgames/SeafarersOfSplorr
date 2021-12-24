#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Ship.h"
namespace data::game
{
	using namespace std::string_literals;
	static const std::string CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [Ships]
		(
			[ShipId] INTEGER PRIMARY KEY AUTOINCREMENT,
			[ShipType] INT NOT NULL,
			[Name] TEXT NOT NULL,
			[X] REAL NOT NULL,
			[Y] REAL NOT NULL,
			[Heading] REAL NOT NULL,
			[Speed] REAL NOT NULL
		);)"s;
	static const std::string QUERY_ITEM = 
		R"(SELECT 
			[ShipId],
			[ShipType],
			[Name],
			[X],
			[Y],
			[Heading],
			[Speed] 
		FROM [Ships] 
		WHERE 
			[ShipId]={};)"s;
	static const std::string QUERY_ITEM_COLUMN =
		R"(SELECT 
			[{}]
		FROM [Ships] 
		WHERE 
			[ShipId]={};)"s;
	static const std::string QUERY_ITEM_XY =
		R"(SELECT 
			[X],
			[Y]
		FROM [Ships] 
		WHERE 
			[ShipId]={};)"s;
	static const std::string INSERT_ITEM =
		R"(INSERT INTO [Ships] 
		(
			[ShipType],
			[Name],
			[X],
			[Y],
			[Heading],
			[Speed]
		) 
		VALUES({},{},{:.4f},{:.4f},{:.4f},{:.4f});)"s;
	static const std::string UPDATE_ITEM = 
		R"(UPDATE [Ships] 
		SET 
			[ShipType]={},
			[Name]={},
			[X]={:4f},
			[Y]={:4f},
			[Heading]={},
			[Speed]={} 
		WHERE 
			[ShipId]={};)"s;
	static const std::string UPDATE_ITEM_COLUMN =
		R"(UPDATE [Ships] 
		SET 
			[{}]={}
		WHERE 
			[ShipId]={};)"s;
	static const std::string UPDATE_ITEM_XY =
		R"(UPDATE [Ships] 
		SET 
			[X]={},
			[Y]={}
		WHERE 
			[ShipId]={};)"s;
	static const std::string DELETE_ALL =
		R"(DELETE FROM [Ships];)"s;
	static const std::string QUERY_ALL = 
		R"(SELECT 
			[ShipId] 
		FROM [Ships];)"s;

	static const std::string FIELD_SHIP_ID = "ShipId";
	static const std::string FIELD_SHIP_TYPE = "ShipType";
	static const std::string FIELD_NAME = "Name";
	static const std::string FIELD_X = "X";
	static const std::string FIELD_Y = "Y";
	static const std::string FIELD_HEADING = "Heading";
	static const std::string FIELD_SPEED = "Speed";

	void Ship::Initialize()
	{
		Common::Execute(CREATE_TABLE);
	}

	int Ship::Write(const Ship& ship)
	{
		Initialize();
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
		Initialize();
		auto records = Common::Execute(QUERY_ITEM, shipId);
		if (!records.empty())
		{
			return ToShip(records.front());
		}
		return std::nullopt;
	}

	void Ship::Clear()
	{
		Initialize();
		Common::Execute(DELETE_ALL);
	}

	std::list<int> Ship::All()
	{
		Initialize();
		std::list<int> result;
		auto records = Common::Execute(QUERY_ALL);
		for (auto& record : records)
		{
			result.push_back(common::Data::ToInt(record[FIELD_SHIP_ID]));
		}
		return result;
	}

	int Ship::Create(int shipType, const std::string& name, const common::XY<double>& location, double heading, double speed)
	{
		Initialize();
		Common::Execute(
			INSERT_ITEM,
			shipType,
			common::Data::QuoteString(name),
			location.GetX(),
			location.GetY(),
			heading,
			speed);
		return Common::LastInsertedIndex();
	}

	std::optional<int> Ship::GetShipType(int shipId)
	{
		Initialize();
		auto record = Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_SHIP_TYPE, shipId);
		if (record)
		{
			return Common::ToInt(record.value(), FIELD_SHIP_TYPE);
		}
		return std::nullopt;
	}

	std::optional<std::string> Ship::GetName(int shipId)
	{
		Initialize();
		auto record = Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_NAME, shipId);
		if (record)
		{
			return Common::ToString(record.value(), FIELD_NAME);
		}
		return std::nullopt;
	}

	std::optional<common::XY<double>> Ship::GetLocation(int shipId)
	{
		Initialize();
		auto record = Common::TryExecuteForOne(QUERY_ITEM_XY, shipId);
		if (record)
		{
			return Common::ToXY(record.value());
		}
		return std::nullopt;
	}

	std::optional<double> Ship::GetHeading(int shipId)
	{
		Initialize();
		auto record = Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_HEADING, shipId);
		if (record)
		{
			return Common::ToDouble(record.value(), FIELD_HEADING);
		}
		return std::nullopt;
	}

	std::optional<double> Ship::GetSpeed(int shipId)
	{
		Initialize();
		auto record = Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_SPEED, shipId);
		if (record)
		{
			return Common::ToDouble(record.value(), FIELD_SPEED);
		}
		return std::nullopt;
	}

	void Ship::SetName(int shipId, const std::string& name)
	{
		Initialize();
		Common::Execute(UPDATE_ITEM_COLUMN, FIELD_NAME, common::Data::QuoteString(name), shipId);
	}

	void Ship::SetLocation(int shipId, const common::XY<double>& location)
	{
		Initialize();
		Common::Execute(UPDATE_ITEM_XY, location.GetX(), location.GetY(), shipId);
	}

	void Ship::SetHeading(int shipId, double heading)
	{
		Initialize();
		Common::Execute(UPDATE_ITEM_COLUMN, FIELD_HEADING, heading, shipId);
	}

	void Ship::SetSpeed(int shipId, double speed)
	{
		Initialize();
		Common::Execute(UPDATE_ITEM_COLUMN, FIELD_SPEED, speed, shipId);
	}
}