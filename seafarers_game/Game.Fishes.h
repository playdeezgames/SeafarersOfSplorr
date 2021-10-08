#pragma once
#include <Common.XY.h>
#include "Game.Fish.h"
#include "Game.Item.h"
#include <functional>
#include <list>
#include <string>
namespace game
{
	struct Fishes
	{
		static std::list<Fish> All();
		static std::list<Fish> AllJunk();
		static const std::string& GetName(const Fish&);
		static std::function<size_t()> GetCountGenerator(const Fish&);
		static std::function<double()> GetRadiusGenerator(const Fish&);
		static std::function<double()> GetSpeedGenerator(const Fish&);
		static std::function<int()> GetStockGenerator(const Fish&);
		static const std::list<common::XY<int>>& GetShape(const Fish&);
		static const common::XY<size_t>& GetSize(const Fish&);
		static Item GetItem(const Fish&);
		static size_t GetJunkWeight(const Fish&);
		static const std::string& GetSprite(const Fish&);
	};
}
