#include <algorithm>
#include <Common.RNG.h>
#include <Data.Game.Delivery.h>
#include <Data.Game.Island.Feature.h>
#include "Game.Session.h"
#include "Game.Session.Island.DeliveryService.h"
namespace game::session::island
{
	void DeliveryService::ApplyTurnEffects() const
	{
		//TODO: eliminate expired jobs
		//TODO: generate new jobs
	}

	void DeliveryService::Populate(const game::Difficulty& difficulty) const
	{
		auto fromIslandId = data::game::island::Feature::ReadIslandId(featureId).value();
		auto candidateIslands = game::Session().GetWorld().GetIslands().GetIslands();
		std::remove_if(
			candidateIslands.begin(),
			candidateIslands.end(),
			[fromIslandId](const auto& island)
			{
				return island.operator int() == fromIslandId;
			});
		auto toIsland = common::RNG::FromVector(candidateIslands).value();
		auto deliveryId = data::game::Delivery::Create(fromIslandId, toIsland.operator int());

		//TODO: populate initial job list
	}

}