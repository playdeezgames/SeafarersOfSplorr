#pragma once
#include "Game.Feature.h"
#include <list>
#include <string>
namespace game//20211011
{
	struct Features
	{
		static const std::list<game::Feature>& All();
		static const std::string& GetName(const game::Feature&);
		static size_t GetMinimumCount(const game::Feature&);
		static double GetCoveragePercentage(const game::Feature&);
	};
}
