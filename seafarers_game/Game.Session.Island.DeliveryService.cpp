#include <algorithm>
#include <Common.RNG.h>
#include <Data.Game.Delivery.h>
#include <Data.Game.Feature.Delivery.h>
#include <Data.Game.Island.Feature.h>
#include "Game.Session.h"
#include "Game.Session.Island.DeliveryService.h"
namespace game::session::island
{

	static void GenerateDeliveryForFeature(int featureId)
	{
		constexpr double TIME_LIMIT_FACTOR = 2.0;
		auto fromIslandId = data::game::island::Feature::ReadIslandId(featureId).value();
		auto candidateIslands = game::Session().GetWorld().GetIslands().GetIslands();
		auto fromIsland = game::Session().GetWorld().GetIslands().GetIsland(fromIslandId);
		auto last = std::remove_if(
			candidateIslands.begin(),
			candidateIslands.end(),
			[fromIslandId](const auto& island)
			{
				return island.operator int() == fromIslandId;
			});
		candidateIslands.erase(last, candidateIslands.end());
		auto toIsland = common::RNG::FromVector(candidateIslands).value();
		int timeLimit = (int)(fromIsland.DistanceTo(toIsland) * TIME_LIMIT_FACTOR);
		auto deliveryId = data::game::Delivery::Create(fromIslandId, toIsland.operator int(), timeLimit);
		data::game::feature::Delivery::Create(featureId, deliveryId);
	}

	void DeliveryService::ApplyTurnEffects() const
	{
		auto deliveries = data::game::feature::Delivery::ReadForFeature(featureId);
		if (deliveries.empty())
		{
			GenerateDeliveryForFeature(featureId);
		}
	}

	void DeliveryService::Populate(const game::Difficulty& difficulty) const
	{
		GenerateDeliveryForFeature(featureId);
	}

	bool DeliveryService::HasDeliveries() const
	{
		return data::game::feature::Delivery::ReadCountForFeature(featureId).value_or(0) > 0;
	}

}