#pragma once
#include <Game.Item.h>
namespace state::in_play
{
	struct IslandBuyQuantity
	{
		static void SetItem(const game::Item&);
		static void Start();
	};
}