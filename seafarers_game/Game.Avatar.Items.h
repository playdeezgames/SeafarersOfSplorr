#pragma once
#include "Game.Difficulty.h"
#include "Game.Item.h"
#include <map>
namespace game::avatar::Items
{
	std::map<game::Item, size_t> All();
	size_t Read(const game::Item&);
	void Add(const game::Item&, const size_t&);
	void Remove(const game::Item&, const size_t&);
	void Reset(const game::Difficulty&);

	double TotalTonnage();
}
