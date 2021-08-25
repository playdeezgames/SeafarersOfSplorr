#include <Common.Data.h>
#include <Common.Heading.h>
#include <Common.RNG.h>
#include <Common.Utility.h>
#include <Common.XY.h>
#include <Data.Game.Island.h>
#include <Data.Game.Merchant.h>
#include "Game.Merchants.h"
namespace game
{
	void Merchants::Spawn(const common::XY<double>& location)
	{
		auto islands = data::game::Island::All();
		size_t index = common::RNG::FromRange(0u, islands.size());
		auto island = common::Utility::GetNth(islands, index);
		data::game::Merchant::Add(
			{ 0, location, island.value().location});
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
				data::game::Merchant::Remove(merchant.merchantId);
			}
		}
	}

	void Merchants::Reset(const Difficulty&)
	{
		data::game::Merchant::Clear();
	}
}