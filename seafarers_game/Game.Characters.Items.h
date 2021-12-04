#pragma once
#include "Game.Difficulty.h"
#include "Game.Item.h"
#include <map>
namespace game::characters
{
	struct Items
	{
		static std::map<game::Item, size_t> All(int);
		static size_t Read(int, const game::Item&);
		static bool Has(int, const Item&);
		static void Add(int, const game::Item&, const size_t&);
		static void Remove(int, const game::Item&, const size_t&);
		static void Reset(int, const game::Difficulty&);
		static double TotalTonnage(int);
	};
}
