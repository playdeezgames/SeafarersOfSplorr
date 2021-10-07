#include <Common.Data.h>
#include <Common.Heading.h>
#include <Common.RNG.h>
#include <Common.Utility.h>
#include <Common.XY.h>
#include <Data.Game.Island.h>
#include <Data.Game.Merchant.h>
#include "Game.Islands.Markets.h"
#include "Game.Items.h"
#include "Game.Merchants.h"
#include "Game.ShipTypes.h"
#include "Game.World.h"
namespace game
{
	static const std::map<ShipType, size_t> shipTypeGenerator =
	{
		{ ShipType::RAFT, 10},
		{ ShipType::JUNK, 1}
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
		auto item = common::RNG::FromGenerator(cargoTypeGenerator, Item::SACK_OF_WHEAT);
		auto ship = common::RNG::FromGenerator(shipTypeGenerator, ShipType::RAFT);
		auto tonnage = ShipTypes::GetTotalTonnage(ship);
		auto cargoQuantity = (size_t)common::RNG::FromRange(0.0, tonnage / Items::GetUnitTonnage(item));
		game::islands::Markets::BuyItems(location, item, cargoQuantity);
		data::game::Merchant::Add({ 
				0, //merchant id is auto assigned
				location, //start
				island.value().location, //destination
				(int)ship,
				(int)item,
				(int)cargoQuantity
			});
	}

	static const double MERCHANT_DOCK_DISTANCE = 0.5;

	static bool MoveMerchant(data::game::Merchant& merchant)
	{
		const double MAXIMUM_STEP = 1.0;
		auto difference = merchant.destination - merchant.location;
		double magnitude = difference.GetMagnitude();
		if (magnitude > MERCHANT_DOCK_DISTANCE)
		{
			double heading = common::Heading::XYToDegrees(difference);
			double stepSize = (magnitude > MAXIMUM_STEP) ? (MAXIMUM_STEP) : (magnitude);
			auto delta = difference * (stepSize / magnitude) * World::GetWindSpeedMultiplier(heading);
			merchant.location = merchant.location + delta;
			data::game::Merchant::Update(merchant);
			return true;
		}
		return false;
	}

	static void SellGoods(data::game::Merchant& merchant)
	{
		game::islands::Markets::SellItems(merchant.location, (game::Item)merchant.cargoType, merchant.cargoQuantity);
		data::game::Merchant::Remove(merchant.merchantId);
	}

	static void ApplyMerchantTurnEffects(data::game::Merchant& merchant)
	{
		if(!MoveMerchant(merchant))
		{
			SellGoods(merchant);
		}
	}

	void Merchants::ApplyTurnEffects()
	{
		auto merchants = data::game::Merchant::All();
		for (auto& merchant : merchants)
		{
			ApplyMerchantTurnEffects(merchant);
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