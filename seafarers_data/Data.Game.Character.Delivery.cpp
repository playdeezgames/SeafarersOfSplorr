#include <algorithm>
#include "Data.Game.Common.h"
#include "Data.Game.Character.h"
#include "Data.Game.Delivery.h"
#include "Data.Game.Character.Delivery.h"
#include <iterator>
namespace data::game::character
{
	using namespace std::string_view_literals;
	static constexpr auto CREATE_TABLE =
		R"(CREATE TABLE IF NOT EXISTS [CharacterDeliveries]
		(
			[CharacterId] INT NOT NULL,
			[DeliveryId] INT NOT NULL UNIQUE,
			FOREIGN KEY ([CharacterId]) REFERENCES [Characters]([CharacterId]),
			FOREIGN KEY ([DeliveryId]) REFERENCES [Deliveries]([DeliveryId])
		);)"sv;
	static constexpr auto REPLACE_ITEM =
		R"(REPLACE INTO [CharacterDeliveries]
		(
			[CharacterId],
			[DeliveryId]
		) 
		VALUES({},{});)"sv;
	static constexpr auto QUERY_DELIVERY_COUNT =
		R"(SELECT 
			COUNT([DeliveryId]) AS [DeliveryCount] 
		FROM [CharacterDeliveries] 
		WHERE 
			[CharacterId]={};)"sv;
	static constexpr auto QUERY_FOR_CHARACTER =
		R"(SELECT 
			[DeliveryId] 
		FROM [CharacterDeliveries] 
		WHERE 
			[CharacterId]={};)"sv;
	static constexpr auto DELETE_ITEM =
		R"(DELETE FROM [CharacterDeliveries] 
		WHERE 
			[DeliveryId]={};)"sv;
	static constexpr auto QUERY_ITEM_COLUMN =
		R"(SELECT 
			[{}] 
		FROM [CharacterDeliveries] 
		WHERE 
			[DeliveryId]={};)"sv;
	static constexpr auto FIELD_DELIVERY_COUNT = "DeliveryCount"sv;
	static constexpr auto FIELD_DELIVERY_ID = "DeliveryId"sv;
	static constexpr auto FIELD_CHARACTER_ID = "CharacterId"sv;

	void Delivery::Initialize()
	{
		data::game::Delivery::Initialize();
		Character::Initialize();
		Common::Execute(CREATE_TABLE);
	}
	
	void Delivery::Create(int characterId, int deliveryId)
	{
		Initialize();
		Common::Execute(REPLACE_ITEM, characterId, deliveryId);
	}

	int Delivery::ReadCount(int characterId)
	{
		Initialize();
		return Common::TryToInt(
			Common::TryExecuteForOne(QUERY_DELIVERY_COUNT, characterId), 
			FIELD_DELIVERY_COUNT)
			.value_or(0);
	}

	std::vector<int> Delivery::Read(int characterId)
	{
		Initialize();
		auto records = Common::Execute(QUERY_FOR_CHARACTER, characterId);
		std::vector<int> result;
		std::transform(
			records.begin(),
			records.end(),
			std::back_inserter(result),
			Common::DoToInt(FIELD_DELIVERY_ID));
		return result;
	}

	void Delivery::Remove(int deliveryId)
	{
		Common::Execute(DELETE_ITEM, deliveryId);
	}

	std::optional<int> Delivery::ReadCharacterId(int deliveryId)
	{
		return Common::TryToInt(
			Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_CHARACTER_ID, deliveryId),
			FIELD_CHARACTER_ID);
	}
}