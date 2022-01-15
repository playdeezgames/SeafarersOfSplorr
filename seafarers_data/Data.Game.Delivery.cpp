#include "Data.Game.Common.h"
#include "Data.Game.Delivery.h"
#include "Data.Game.Island.h"
namespace data::game
{
	using namespace std::string_view_literals;
	static constexpr auto CREATE_TABLE =
		R"(CREATE TABLE IF NOT EXISTS [Deliveries]
		(
			[DeliveryId] INTEGER PRIMARY KEY AUTOINCREMENT,
			[FromIslandId] INT NOT NULL,
			[ToIslandId] INT NOT NULL,
			FOREIGN KEY ([FromIslandId]) REFERENCES [Islands]([IslandId]),
			FOREIGN KEY ([ToIslandId]) REFERENCES [Islands]([IslandId])
		);)"sv;
	static constexpr auto INSERT_ITEM =
		R"(INSERT INTO [Deliveries]
		(
			[FromIslandId],
			[ToIslandId]
		) 
		VALUES({},{});)"sv;
	static constexpr auto QUERY_ITEM_COLUMN =
		R"(SELECT 
			[{}] 
		FROM [Deliveries] 
		WHERE 
			[DeliveryId]={};)"sv;
	static constexpr auto DELETE_ITEM =
		R"()"sv;
	static constexpr auto FIELD_TO_ISLAND_ID = "ToIslandId"sv;
	static constexpr auto FIELD_FROM_ISLAND_ID = "FromIslandId"sv;

	void Delivery::Initialize()
	{
		Island::Initialize();
		Common::Execute(CREATE_TABLE);
	}

	int Delivery::Create(int fromIslandId, int toIslandId)
	{
		Initialize();
		Common::Execute(INSERT_ITEM, fromIslandId, toIslandId);
		return Common::LastInsertedIndex();
	}

	std::optional<int> Delivery::ReadToIsland(int deliveryId)
	{
		return Common::TryToInt(
			Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_TO_ISLAND_ID, deliveryId),
			FIELD_TO_ISLAND_ID);
	}

	std::optional<int> Delivery::ReadFromIsland(int deliveryId)
	{
		return Common::TryToInt(
			Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_FROM_ISLAND_ID, deliveryId),
			FIELD_FROM_ISLAND_ID);
	}

	void Delivery::Remove(int deliveryId)
	{
		Common::Execute(DELETE_ITEM, deliveryId);
	}
}