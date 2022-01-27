#include "Game.Session.Island.DeliveryService.h"
#include "Game.Session.World.h"
#include "Game.Session.Islands.h"
#include <Common.RNG.h>
#include <Data.Game.Delivery.h>
#include <Data.Game.Feature.Delivery.h>
#include <Data.Game.Island.Feature.h>
namespace game::session::island
{

	static void GenerateDeliveryForFeature(int featureId)
	{
		constexpr double TIME_LIMIT_FACTOR = 2.0;
		constexpr double MINIMUM_REWARD_VALUE = 1.0;
		constexpr double MAXIMUM_REWARD_VALUE = 5.0;
		using world = game::session::World;
		using islands = game::session::Islands;
		auto fromIslandId = data::game::island::Feature::ReadIslandId(featureId).value();
		auto candidateIslands = islands::GetIslands();
		auto fromIsland = islands::GetIsland(fromIslandId);
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
		auto rewardValue = common::RNG::FromRange(MINIMUM_REWARD_VALUE, MAXIMUM_REWARD_VALUE);
		//TODO: make this less hacked
		auto deliveryId = data::game::Delivery::Create(fromIslandId, toIsland.operator int(), timeLimit, rewardValue);
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

	void DeliveryService::Populate(const game::Difficulty&) const
	{
		GenerateDeliveryForFeature(featureId);
	}

	bool DeliveryService::HasDeliveries() const
	{
		return data::game::feature::Delivery::ReadCountForFeature(featureId).value_or(0) > 0;
	}

}