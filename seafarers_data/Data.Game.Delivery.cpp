#include "Data.Game.Common.h"
#include "Data.Game.Delivery.h"
#include "Data.Game.Island.h"
namespace data::game
{
	using namespace std::string_view_literals;
	static constexpr std::string_view CREATE_TABLE =
		R"(CREATE TABLE IF NOT EXISTS [Deliveries]
		(
			[DeliveryId] INTEGER PRIMARY KEY AUTOINCREMENT,
			[FromIslandId] INT NOT NULL,
			[ToIslandId] INT NOT NULL,
			FOREIGN KEY ([FromIslanId]) REFERENCES [Islands]([IslandId]),
			FOREIGN KEY ([ToIslandId]) REFERENCES [Islands]([IslandId])
		);)"sv;
	static constexpr std::string_view INSERT_ITEM =
		R"(INSERT INTO [Deliveries]
		(
			[FromIslandId],
			[ToIslandId]
		) 
		VALUES({},{});)"sv;

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
}