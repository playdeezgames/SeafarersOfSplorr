#include <Common.Heading.h>
#include <Common.RNG.h>
#include <Common.Utility.h>
#include <Common.Utility.List.h>
#include <Data.Game.Fishery.h>
#include <functional>
#include "Game.Ship.h"
#include "Game.Fishes.h"
#include "Game.Fisheries.h"
#include "Game.World.h"
namespace game//20211015
{
	static void GenerateFishery(const Fish& fish)
	{
		auto worldSize = World::GetSize();
		double speed = Fishes::GenerateSpeed(fish);
		double radians = common::Heading::ToRadians(common::RNG::FromRange(0.0, common::Heading::DEGREES));
		int stock = Fishes::GenerateStock(fish);
		data::game::Fishery::Add({
			0,//fisheryid -- given a value by the data store... here 0 is just a placeholder
			(int)fish,
			{
				common::RNG::FromRange(0.0, worldSize.GetX()), 
				common::RNG::FromRange(0.0, worldSize.GetY())
			},
			{
				speed * std::cos(radians),
				speed * std::sin(radians)
			},
			Fishes::GenerateRadius(fish),
			stock,
			0
		});
	}

	static void GenerateFisheries(const Fish& fish)
	{
		size_t fisheryCount = Fishes::GenerateCount(fish);
		while (fisheryCount > 0)
		{
			GenerateFishery(fish);
			fisheryCount--;
		}
	}

	void Fisheries::Reset(const Difficulty&)
	{
		data::game::Fishery::Clear();
		for (auto& fish : game::Fishes::All())
		{
			GenerateFisheries(fish);
		}
	}

	void Fisheries::ApplyTurnEffects()
	{
		auto worldSize = World::GetSize();
		for (auto& fishery : data::game::Fishery::All())
		{
			fishery.location = fishery.location + fishery.movement;
			if (fishery.location.GetX() < 0.0)//bounce off west edge
			{
				fishery.movement.ReflectX();
			}
			if (fishery.location.GetY() < 0.0)//bounce off north edge
			{
				fishery.movement.ReflectY();
			}
			if (fishery.location.GetX() >= worldSize.GetX())//bounce off east edge
			{
				fishery.movement.ReflectX();
			}
			if (fishery.location.GetY() >=worldSize.GetY())//bounce off south edge
			{
				fishery.movement.ReflectY();
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
		return common::utility::List::Map<data::game::Fishery, Fishery>(data::game::Fishery::All, ToFishery);
	}

	std::list<Fishery> Fisheries::Available()
	{
		auto avatarLocation = Ship::GetLocation();
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