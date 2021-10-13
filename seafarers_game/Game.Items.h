#pragma once
#include "Game.Commodity.h"
#include "Game.Difficulty.h"
#include "Game.Item.h"
#include <list>
#include <map>
#include <string>
namespace game//20211011
{
	struct Items
	{
		static const std::list<game::Item>& All();
		static game::Item GenerateRationsForAvatar();
		static const std::string& GetName(const game::Item&);
		static bool GeneratePresenceForIsland(const game::Item&);//for island
		static const std::map<Commodity, double>& GetCommodities(const game::Item&);
		static double GetUnitTonnage(const game::Item&);
		static const std::map<Difficulty, size_t>& GetInitialInventories(const game::Item&);//for avatar
	};
}
