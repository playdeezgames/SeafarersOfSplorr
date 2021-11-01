#pragma once
#include <Game.Item.h>
namespace state::in_play
{
	struct IslandSellQuantity
	{
		static void SetItem(const game::Item&);
		static void Start();
	};
}