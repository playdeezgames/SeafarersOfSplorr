#include "Data.Game.Ship.Counter.h"
#include "Data.Game.Common.h"
#include "Data.Game.Ship.h"
namespace data::game::ship
{
	using namespace std::string_view_literals;

	static constexpr auto CREATE_TABLE =
		R"(CREATE TABLE IF NOT EXISTS [ShipCounters]
		(
			[ShipId] INT NOT NULL,
			[Counter] INT NOT NULL,
			[Value] INT NOT NULL,
			UNIQUE([ShipId],[Counter]),
			FOREIGN KEY ([ShipId]) REFERENCES [Ships]([ShipId])
		);)"sv;
	static constexpr auto REPLACE_ITEM =
		R"(REPLACE INTO [ShipCounters]
		(
			[ShipId],
			[Counter],
			[Value]
		) 
		VALUES({},{},{});)"sv;
	static constexpr auto QUERY_ITEM =
		R"(SELECT 
			[Value] 
		FROM [ShipCounters] 
		WHERE 
			[ShipId]={} 
			AND [Counter]={};)"sv;
	static constexpr auto COLUMN_VALUE = "Value"sv;

	void Counter::Initialize()
	{
		Ship::Initialize();
		Common::Execute(CREATE_TABLE);
	}

	std::optional<int> Counter::Read(int shipId, int counter)
	{
		Initialize();
		return Common::TryToInt(
			Common::TryExecuteForOne(QUERY_ITEM, shipId, counter), COLUMN_VALUE);
	}

	void Counter::Write(int shipId, int counter, int value)
	{
		Initialize();
		Common::Execute(REPLACE_ITEM, shipId, counter, value);
	}
}