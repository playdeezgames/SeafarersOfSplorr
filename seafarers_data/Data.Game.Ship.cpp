#include <algorithm>
#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Ship.h"
#include <iterator>
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

	void Ship::Clear()
	{
		Initialize();
		Common::Execute(DELETE_ALL);
	}

	std::vector<int> Ship::All()
	{
		Initialize();
		auto records = Common::Execute(QUERY_ALL);
		std::vector<int> result(records.size());
		std::transform(
			records.begin(),
			records.end(),
			std::back_inserter(result),
			Common::DoToInt(FIELD_SHIP_ID));
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
		return Common::TryToInt(
			Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_SHIP_TYPE, shipId), 
			FIELD_SHIP_TYPE);
	}

	std::optional<std::string> Ship::GetName(int shipId)
	{
		Initialize();
		return Common::TryToString(
			Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_NAME, shipId),
			FIELD_NAME);
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
		return Common::TryToDouble(
			Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_HEADING, shipId),
			FIELD_HEADING);
	}

	std::optional<double> Ship::GetSpeed(int shipId)
	{
		Initialize();
		return Common::TryToDouble(
			Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_SPEED, shipId),
			FIELD_SPEED);
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