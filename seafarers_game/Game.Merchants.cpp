#include <Common.Data.h>
#include <Common.Heading.h>
#include <Common.RNG.h>
#include <Common.Utility.h>
#include <Common.XY.h>
#include <Data.Game.Island.h>
#include <Data.Game.Merchant.h>
#include "Game.Merchants.h"
#include "Game.Ship.h"
#include "Game.Item.h"
namespace game
{
	static const std::map<Ship, size_t> shipTypeGenerator =
	{
		{ Ship::RAFT, 10},
		{ Ship::JUNK, 1}
	};

	static const std::map<Item, size_t> cargoTypeGenerator =
	{
		{ Item::SACK_OF_WHEAT, 3},
		{ Item::CLOTH, 1}
	};

	void Merchants::Spawn(const common::XY<double>& location)
	{
		auto islands = data::game::Island::All();
		size_t index = common::RNG::FromRange(0u, islands.size());
		auto island = common::Utility::GetNth(islands, index);
		data::game::Merchant::Add({ 
				0, //merchant id is auto assigned
				location, //start
				island.value().location, //destination
				(int)common::RNG::FromGenerator(shipTypeGenerator, Ship::RAFT)
			});
	}

	void Merchants::ApplyTurnEffects()
	{
		const double MAXIMUM_STEP = 1.0;
		auto merchants = data::game::Merchant::All();
		for (auto& merchant : merchants)
		{
			auto difference = merchant.destination - merchant.location;
			double magnitude = difference.GetMagnitude();
			if (magnitude > 0.0)
			{
				double stepSize = (magnitude > MAXIMUM_STEP) ? (MAXIMUM_STEP) : (magnitude);
				auto delta = difference * (stepSize / magnitude);
				merchant.location = merchant.location + delta;
				data::game::Merchant::Update(merchant);
			}
			else
			{
				//TODO: sell goods on board at destination island
				data::game::Merchant::Remove(merchant.merchantId);
			}
		}
	}

	void Merchants::Reset(const Difficulty&)
	{
		data::game::Merchant::Clear();
	}

	std::list<Merchant> Merchants::All()
	{
		std::list<Merchant> result;
		auto merchants = data::game::Merchant::All();
		for (auto& merchant : merchants)
		{
			result.push_back({
				merchant.location
				});
		}
		return result;
	}

}