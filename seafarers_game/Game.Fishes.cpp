#include <Common.Utility.h>
#include <Common.RNG.h>
#include <functional>
#include "Game.Fishes.h"
#include <map>
namespace game
{
	struct FishDescriptor
	{
		std::string name;
		std::function<size_t()> countGenerator;
		std::function<double()> radiusGenerator;
		std::function<double()> speedGenerator;
		std::function<int()> stockGenerator;
		std::list<common::XY<int>> shape;
		common::XY<size_t> size;
		Item item;
		size_t junkWeight;
		std::string sprite;
	};

	static const std::map<Fish, FishDescriptor> descriptors =
	{
		{
			Fish::MARLIN,
			{
				"marlin",
				[]() { return common::RNG::FromRange(1,3); },
				[]() { return common::RNG::FromRange(2.5,5.0); },
				[]() { return common::RNG::FromRange(0.1,0.2); },
				[]() { return 100; },
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
				[]() { return 100; },
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
				[]() { return 100; },
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
				[]() { return 0; },
				{
					{0,0}
				},
				{ 1, 1 },
				Item::TROUSERS,
				1,
				"FishboardFishTrousers"
			}
		},
		{
			Fish::SEAWEED,
			{
				"seaweed",
				[]() { return 0; },
				[]() { return 0.0; },
				[]() { return 0.0; },
				[]() { return 0; },
				{
					{0,0}
				},
				{ 1, 1 },
				Item::SEAWEED,
				99,
				"FishboardFishSeaweed"
			}
		}
	};

	static std::list<Fish> fishes;
	static std::list<Fish> junk;

	std::list<Fish> Fishes::All()
	{
		return common::Utility::ListFromTable(fishes, descriptors);
	}

	std::list<Fish> Fishes::AllJunk()
	{
		if (junk.empty())
		{
			for (auto& descriptor : descriptors)
			{
				if (descriptor.second.junkWeight > 0)
				{
					junk.push_back(descriptor.first);
				}
			}
		}
		return junk;
	}

	static const FishDescriptor& Read(const Fish& fish)
	{
		return descriptors.find(fish)->second;
	}

	const std::string& Fishes::GetName(const Fish& fish)
	{
		return Read(fish).name;
	}

	size_t Fishes::GenerateCount(const Fish& fish)
	{
		return Read(fish).countGenerator();
	}

	double Fishes::GenerateRadius(const Fish& fish)
	{
		return Read(fish).radiusGenerator();
	}

	double Fishes::GenerateSpeed(const Fish& fish)
	{
		return Read(fish).speedGenerator();

	}

	int Fishes::GenerateStock(const Fish& fish)
	{
		return Read(fish).stockGenerator();

	}

	const std::list<common::XY<int>>& Fishes::GetShape(const Fish& fish)
	{
		return Read(fish).shape;

	}

	const common::XY<size_t>& Fishes::GetSize(const Fish& fish)
	{
		return Read(fish).size;

	}

	Item Fishes::GetItem(const Fish& fish)
	{
		return Read(fish).item;

	}

	size_t Fishes::GetJunkWeight(const Fish& fish)
	{
		return Read(fish).junkWeight;

	}

	const std::string& Fishes::GetSprite(const Fish& fish)
	{
		return Read(fish).sprite;

	}

}