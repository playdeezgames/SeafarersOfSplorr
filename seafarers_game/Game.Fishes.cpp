#include <Common.RNG.h>
#include "Game.Fishes.h"
#include <map>
namespace game
{
	static const std::map<Fish, FishDescriptor> descriptors =
	{
		{
			Fish::MARLIN,
			{
				"marlin",
				[]() { return common::RNG::FromRange(1,3); },
				[]() { return common::RNG::FromRange(2.5,5.0); },
				[]() { return common::RNG::FromRange(0.1,0.2); },
				{
					{0,0},{1,0}
				},
				{ 2, 1 },
				Item::FISH,
				0,
				"FishboardFishRed"
			}
		},
		{
			Fish::COD,
			{
				"cod",
				[]() { return common::RNG::FromRange(5,11); },
				[]() { return common::RNG::FromRange(5.0,10.0); },
				[]() { return common::RNG::FromRange(0.1,0.2); },
				{
					{0,0},{0,1}
				},
				{ 1, 2 },
				Item::FISH,
				0,
				"FishboardFishPurple"
			}
		},
		{
			Fish::TUNA,
			{
				"tuna",
				[]() { return common::RNG::FromRange(2,5); },
				[]() { return common::RNG::FromRange(3.0,6.0); },
				[]() { return common::RNG::FromRange(0.1,0.2); },
				{
					{0,0},{1,1}
				},
				{ 2, 2 },
				Item::FISH,
				0,
				"FishboardFishGreen"
			}
		},
		{
			Fish::TROUSERS,
			{
				"trousers",
				[]() { return 0; },
				[]() { return 0.0; },
				[]() { return 0.0; },
				{
					{0,0}
				},
				{ 1, 1 },
				Item::TROUSERS,
				1,
				"FishboardFishTrousers"
			}
		}
	};

	static std::list<Fish> fishes;

	std::list<Fish> Fishes::All()
	{
		if (fishes.empty())
		{
			for (auto& descriptor : descriptors)
			{
				fishes.push_back(descriptor.first);
			}
		}
		return fishes;
	}

	const FishDescriptor& Fishes::Read(const Fish& fish)
	{
		return descriptors.find(fish)->second;
	}
}