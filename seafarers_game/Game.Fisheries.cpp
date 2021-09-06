#include <Common.Heading.h>
#include <Common.RNG.h>
#include <Data.Game.Fishery.h>
#include "Game.Fishes.h"
#include "Game.Fisheries.h"
#include "Game.World.h"
namespace game
{
	static void GenerateFishery(const Fish& fish, const FishDescriptor& descriptor)
	{
		auto worldSize = World::GetSize();
		double radius = descriptor.radiusGenerator();
		double speed = descriptor.speedGenerator();
		double radians = common::Heading::ToRadians(common::RNG::FromRange(0.0, common::Heading::DEGREES));
		data::game::Fishery::Add({
			0,
			(int)fish,
			{
				common::RNG::FromRange(0.0, worldSize.GetX()), 
				common::RNG::FromRange(0.0, worldSize.GetY())
			},
			{
				speed * std::cos(radians),
				speed * std::sin(radians)
			},
			radius
		});
	}

	static void GenerateFisheries(const Fish& fish, const FishDescriptor& descriptor)
	{
		size_t fisheryCount = descriptor.countGenerator();
		while (fisheryCount > 0)
		{
			GenerateFishery(fish, descriptor);
			fisheryCount--;
		}
	}

	void Fisheries::Reset(const Difficulty&)
	{
		data::game::Fishery::Clear();
		for (auto& fish : game::Fishes::All())
		{
			auto& descriptor = game::Fishes::Read(fish);
			GenerateFisheries(fish, descriptor);
		}
	}

	void Fisheries::ApplyTurnEffects()
	{

	}

	std::list<Fishery> Fisheries::All()
	{
		std::list<Fishery> result;
		for (auto& fishery : data::game::Fishery::All())
		{
			result.push_back({ fishery.location });
		}
		return result;
	}
}