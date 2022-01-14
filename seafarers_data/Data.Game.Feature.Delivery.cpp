#include <algorithm>
#include "Data.Game.Common.h"
#include "Data.Game.Island.Feature.h"
#include "Data.Game.Delivery.h"
#include "Data.Game.Feature.Delivery.h"
#include <iterator>
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
	static constexpr std::string_view QUERY_FOR_FEATURE =
		R"(SELECT 
			[DeliveryId] 
		FROM [FeatureDeliveries] 
		WHERE 
			[FeatureId]={};)"sv;
	static constexpr std::string_view CLEAR_ITEM =
		R"(DELETE FROM [FeatureDeliveries] 
		WHERE 
			[DeliveryId]={};)"sv;

	static constexpr std::string_view FIELD_DELIVERY_ID = "DeliveryId"sv;

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

	std::vector<int> Delivery::ReadForFeature(int featureId)
	{
		Initialize();
		auto records = Common::Execute(QUERY_FOR_FEATURE, featureId);
		std::vector<int> result;
		std::transform(
			records.begin(), 
			records.end(), 
			std::back_inserter(result), 
			Common::DoToInt(FIELD_DELIVERY_ID));
		return result;
	}

	void Delivery::Clear(int deliveryId)
	{
		Initialize();
		Common::Execute(CLEAR_ITEM, deliveryId);
	}
}