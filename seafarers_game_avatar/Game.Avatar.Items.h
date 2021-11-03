#pragma once
#include "Game.Difficulty.h"
#include "Game.Item.h"
#include <map>
namespace game::avatar//20211017
{
	struct Items
	{
		static std::map<game::Item, size_t> All(int);
		static size_t Read(int, const game::Item&);
		static bool Has(int, const Item&);
		static void Add(int, const game::Item&, const size_t&);
		static void Remove(int, const game::Item&, const size_t&);
		static void Reset(const game::Difficulty&);
		static double TotalTonnage(int);
	};
}
