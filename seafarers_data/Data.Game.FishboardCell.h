#pragma once
#include <Common.XY.h>
#include <list>
#include <optional>
#include <set>
namespace data::game
{
	struct FishboardCell
	{
		common::XY<int> location;
		bool revealed;
		std::optional<int> fishType;

		static void Clear(int);
		static void Write(int, const FishboardCell&);
		static std::list<FishboardCell> All(int);
		static std::optional<FishboardCell> Read(int, const common::XY<int>&);

		static size_t ReadFishCount(int);
		static size_t ReadRevealedFishCount(int);
		static std::set<int> ReadFish(int);
	};
}
