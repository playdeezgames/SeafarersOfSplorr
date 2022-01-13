#include "Data.Game.Common.h"
#include "Data.Game.Island.Feature.h"
#include "Data.Game.Delivery.h"
#include "Data.Game.Feature.Delivery.h"
namespace data::game::feature
{
	using namespace std::string_view_literals;
	static constexpr std::string_view CREATE_TABLE =
		R"(CREATE TABLE IF NOT EXISTS [FeatureDeliveries]
		(
			[FeatureId] INT NOT NULL,
			[DeliveryId] INT NOT NULL UNIQUE,
			FOREIGN KEY ([FeatureId]) REFERENCES [Features]([FeatureId]),
			FOREIGN KEY ([DeliveryId]) REFERENCES [Deliveries]([DeliveryId])
		);)"sv;
	static constexpr std::string_view REPLACE_ITEM =
		R"(REPLACE INTO [FeatureDeliveries]
		(
			[FeatureId],
			[DeliveryId]
		) 
		VALUES({},{});)"sv;

	void Delivery::Initialize()
	{
		island::Feature::Initialize();
		data::game::Delivery::Initialize();
		Common::Execute(CREATE_TABLE);
	}

	void Delivery::Create(int featureId, int deliveryId)
	{
		Initialize();
		Common::Execute(REPLACE_ITEM, featureId, deliveryId);
	}
}