#include <Common.RNG.h>
#include <Common.Utility.h>
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
			{ location, island.value().location});
	}
}