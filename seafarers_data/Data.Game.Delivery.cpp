#include "Data.Game.Common.h"
#include "Data.Game.Item.Type.h"
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
			[TimeLimit] INT NOT NULL,
			[RewardItemTypeId] INT NOT NULL,
			[RewardQuantity] INT NOT NULL,
			[RewardValue] REAL NOT NULL,
			FOREIGN KEY ([FromIslandId]) REFERENCES [Islands]([IslandId]),
			FOREIGN KEY ([ToIslandId]) REFERENCES [Islands]([IslandId]),
			FOREIGN KEY ([RewardItemTypeId]) REFERENCES [ItemTypes]([ItemTypeId])
		);)"sv;
	static constexpr auto INSERT_ITEM =
		R"(INSERT INTO [Deliveries]
		(
			[FromIslandId],
			[ToIslandId],
			[TimeLimit],
			[RewardItemTypeId],
			[RewardQuantity],
			[RewardValue]
		) 
		VALUES({},{},{},{},{},{});)"sv;
	static constexpr auto QUERY_ITEM_COLUMN =
		R"(SELECT 
			[{}] 
		FROM [Deliveries] 
		WHERE 
			[DeliveryId]={};)"sv;
	static constexpr auto DELETE_ITEM =
		R"(DELETE FROM [Deliveries] 
		WHERE 
			[DeliveryId]={})"sv;
	static constexpr auto UPDATE_TIME_LIMIT =
		R"(UPDATE [Deliveries] 
		SET 
			[TimeLimit]={} 
		WHERE 
			[DeliveryId]={};)"sv;
	static constexpr auto FIELD_TO_ISLAND_ID = "ToIslandId"sv;
	static constexpr auto FIELD_FROM_ISLAND_ID = "FromIslandId"sv;
	static constexpr auto FIELD_TIME_LIMIT = "TimeLimit"sv;
	static constexpr auto FIELD_REWARD_QUANTITY = "RewardQuantity"sv;
	static constexpr auto FIELD_REWARD_ITEM_TYPE_ID = "RewardItemTypeId"sv;
	static constexpr auto FIELD_REWARD_VALUE = "RewardValue"sv;

	void Delivery::Initialize()
	{
		item::Type::Initialize();
		Island::Initialize();
		Common::Execute(CREATE_TABLE);
	}

	int Delivery::Create(
		int fromIslandId, 
		int toIslandId, 
		int timeLimit, 
		int rewardItemType, 
		int rewardQuantity,
		double rewardValue)
	{
		Initialize();
		Common::Execute(
			INSERT_ITEM, 
			fromIslandId, 
			toIslandId, 
			timeLimit, 
			rewardItemType, 
			rewardQuantity,
			rewardValue);
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

	std::optional<int> Delivery::ReadTimeLimit(int deliveryId)
	{
		return Common::TryToInt(
			Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_TIME_LIMIT, deliveryId),
			FIELD_TIME_LIMIT);
	}

	std::optional<int> Delivery::ReadRewardQuantity(int deliveryId)
	{
		return Common::TryToInt(
			Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_REWARD_QUANTITY, deliveryId),
			FIELD_REWARD_QUANTITY);
	}

	std::optional<int> Delivery::ReadRewardItemTypeId(int deliveryId)
	{
		return Common::TryToInt(
			Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_REWARD_ITEM_TYPE_ID, deliveryId),
			FIELD_REWARD_ITEM_TYPE_ID);
	}

	std::optional<double> Delivery::ReadRewardValue(int deliveryId)
	{
		return Common::TryToDouble(
			Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_REWARD_VALUE, deliveryId),
			FIELD_REWARD_VALUE);
	}


	void Delivery::WriteTimeLimit(int deliveryId, int timeLimit)
	{
		Common::Execute(UPDATE_TIME_LIMIT, timeLimit, deliveryId);
	}

	void Delivery::Remove(int deliveryId)
	{
		Common::Execute(DELETE_ITEM, deliveryId);
	}
}