#include <Common.Heading.h>
#include <Common.RNG.h>
#include <Data.Game.Fishery.h>
#include "Game.Avatar.AtSea.h"
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
		int stock = descriptor.stockGenerator();
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
			radius,
			stock,
			0
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
		auto worldSize = World::GetSize();
		for (auto& fishery : data::game::Fishery::All())
		{
			fishery.location = fishery.location + fishery.movement;
			if (fishery.location.GetX() < 0.0)
			{
				fishery.movement = { std::abs(fishery.movement.GetX()), fishery.movement.GetY() };
			}
			if (fishery.location.GetY() < 0.0)
			{
				fishery.movement = { fishery.movement.GetX(), std::abs(fishery.movement.GetY()) };
			}
			if (fishery.location.GetX() >= worldSize.GetX())
			{
				fishery.movement = { -std::abs(fishery.movement.GetX()), fishery.movement.GetY() };
			}
			if (fishery.location.GetY() >=worldSize.GetY())
			{
				fishery.movement = { fishery.movement.GetX(), -std::abs(fishery.movement.GetY()) };
			}
			data::game::Fishery::Write(fishery);
		}
	}

	static Fishery ToFishery(const data::game::Fishery& fishery)
	{
		return {
				fishery.fisheryId,
				fishery.location,
				fishery.radius,
				(Fish)fishery.fishType,
				(size_t)fishery.stock,
				(size_t)fishery.depletion };
	}

	std::list<Fishery> Fisheries::All()
	{
		std::list<Fishery> result;
		for (auto& fishery : data::game::Fishery::All())
		{
			result.push_back(ToFishery(fishery));
		}
		return result;
	}

	std::list<Fishery> Fisheries::Available()
	{
		auto avatarLocation = avatar::AtSea::GetLocation();
		std::list<Fishery> result;
		for (auto& fishery : All())
		{
			auto distance = (fishery.location - avatarLocation).GetMagnitude();
			if (distance <= fishery.radius)
			{
				result.push_back(fishery);
			}
		}
		return result;
	}

	std::optional<Fishery> Fisheries::Read(int fisheryId)
	{
		auto fishery = data::game::Fishery::Read(fisheryId);
		if (fishery)
		{
			return ToFishery(fishery.value());
		}
		return std::nullopt;
	}

}