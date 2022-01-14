#include "Data.Game.Common.h"
#include "Data.Game.Character.h"
#include "Data.Game.Delivery.h"
#include "Data.Game.Character.Delivery.h"
namespace data::game::character
{
	using namespace std::string_view_literals;
	static constexpr std::string_view CREATE_TABLE =
		R"(CREATE TABLE IF NOT EXISTS [CharacterDeliveries]
		(
			[CharacterId] INT NOT NULL,
			[DeliveryId] INT NOT NULL UNIQUE,
			FOREIGN KEY ([CharacterId]) REFERENCES [Characters]([CharacterId]),
			FOREIGN KEY ([DeliveryId]) REFERENCES [Deliveries]([DeliveryId])
		);)"sv;
	static constexpr std::string_view REPLACE_ITEM =
		R"(REPLACE INTO [CharacterDeliveries]
		(
			[CharacterId],
			[DeliveryId]
		) 
		VALUES({},{});)"sv;

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
}