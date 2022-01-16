#include <algorithm>
#include "Data.Game.Common.h"
#include "Data.Game.Island.Feature.h"
#include "Data.Game.Delivery.h"
#include "Data.Game.Feature.Delivery.h"
#include <iterator>
namespace data::game::feature
{
	using namespace std::string_view_literals;
	static constexpr auto CREATE_TABLE =
		R"(CREATE TABLE IF NOT EXISTS [FeatureDeliveries]
		(
			[FeatureId] INT NOT NULL,
			[DeliveryId] INT NOT NULL UNIQUE,
			FOREIGN KEY ([FeatureId]) REFERENCES [Features]([FeatureId]),
			FOREIGN KEY ([DeliveryId]) REFERENCES [Deliveries]([DeliveryId])
		);)"sv;
	static constexpr auto REPLACE_ITEM =
		R"(REPLACE INTO [FeatureDeliveries]
		(
			[FeatureId],
			[DeliveryId]
		) 
		VALUES({},{});)"sv;
	static constexpr auto QUERY_FOR_FEATURE =
		R"(SELECT 
			[DeliveryId] 
		FROM [FeatureDeliveries] 
		WHERE 
			[FeatureId]={};)"sv;
	static constexpr auto CLEAR_ITEM =
		R"(DELETE FROM [FeatureDeliveries] 
		WHERE 
			[DeliveryId]={};)"sv;
	static constexpr auto QUERY_COUNT_FOR_FEATURE =
		R"(SELECT 
			COUNT([DeliveryId]) AS [DeliveryCount] 
		FROM [FeatureDeliveries] 
		WHERE 
			[FeatureId]={};)"sv;

	static constexpr auto FIELD_DELIVERY_ID = "DeliveryId"sv;
	static constexpr auto FIELD_DELIVERY_COUNT = "DeliveryCount"sv;

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

	std::optional<int> Delivery::ReadCountForFeature(int featureId)
	{
		Initialize();
		return Common::TryToInt(
			Common::TryExecuteForOne(QUERY_COUNT_FOR_FEATURE, featureId),
			FIELD_DELIVERY_COUNT);
	}


	void Delivery::Clear(int deliveryId)
	{
		Initialize();
		Common::Execute(CLEAR_ITEM, deliveryId);
	}
}