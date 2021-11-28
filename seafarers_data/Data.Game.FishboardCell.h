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

		static void Clear();
		static void Write(const FishboardCell&);
		static std::list<FishboardCell> All();
		static std::optional<FishboardCell> Read(const common::XY<int>&);

		static size_t ReadFishCount();
		static size_t ReadRevealedFishCount();
		static std::set<int> ReadFish();
	};
}
