#include <Common.Heading.h>
#include <Data.Game.Character.KnownIsland.h>
#include <Data.Game.Island.h>
#include "Game.Characters.Ships.h"
#include "Game.Islands.h"
#include "Game.Session.h"
#include "Game.Ship.h"
#include "Game.Ship.Crew.h"
namespace game
{
	const std::string Islands::UNKNOWN = "????";

	void GenerateIslands();

	void Islands::Reset(const game::Difficulty&)
	{
		GenerateIslands();
	}

	static void KnowIsland(
		int characterId,
		std::list<Island>& accumulator, 
		const data::game::Island& island, 
		const common::XY<double>& shipLocation)
	{
		data::game::character::KnownIsland::Write(characterId, island.id);
		accumulator.push_back(
			{
				island.id,
				(island.location - shipLocation),
				island.location,
				island.name,
				true,
				island.patronDemigodId
			});
	}

	static void KnowIslandWhenCloseEnough(int characterId, std::list<Island>& accumulator, const data::game::Island& island, const common::XY<double>& shipLocation, std::function<bool(const data::game::Island&, double)> filter)
	{
		auto distance = common::Heading::Distance(shipLocation, island.location);
		if (filter(island, distance))
		{
			KnowIsland(characterId, accumulator, island, shipLocation);
		}
	}

	static std::list<Island> GetIslandsInRange(int shipId, std::function<bool(const data::game::Island&, double)> filter)
	{
		std::list<Island> accumulator;
		auto shipLocation = game::Ship::GetLocation(shipId).value();;
		auto islands = data::game::Island::All();
		auto crew = game::ship::Crew::Read(shipId);
		for (auto crewMember : crew)
		{
			for (auto& island : islands)
			{
				KnowIslandWhenCloseEnough(crewMember.avatarId, accumulator, island, shipLocation, filter);
			}
		}
		return accumulator;
	}

	static std::function<bool(const data::game::Island&, double)> FixedDistance(double maximumDistance)
	{
		return [maximumDistance](const data::game::Island&, double distance)
		{
			return distance <= maximumDistance;
		};
	}

	std::list<Island> Islands::GetViewableIslands(int shipId)
	{
		return GetIslandsInRange(shipId, FixedDistance(game::Session().GetWorld().GetDistances().GetView()));
	}

	std::list<Island> Islands::GetDockableIslands(int shipId)
	{
		return GetIslandsInRange(shipId, FixedDistance(game::Session().GetWorld().GetDistances().GetDock()));
	}

	bool Islands::CanDock(int shipId)
	{
		return !GetDockableIslands(shipId).empty();
	}

	void Islands::SetKnown(int characterId, int islandId, int turn)
	{
		data::game::character::KnownIsland::Write(characterId, islandId);
	}

	static Island ToIsland(const data::game::Island& island, bool isKnown)
	{
		return 
		{
			island.id,
			{0.0, 0.0},
			island.location,
			island.name,
			isKnown,
			island.patronDemigodId
		};
	}

	std::optional<Island> Islands::Read(int characterId, int islandId)
	{
		auto island = data::game::Island::Read(islandId);
		if (island)
		{
			return ToIsland(
				island.value(),
				data::game::character::KnownIsland::Read(characterId, islandId));
		}
		return std::nullopt;
	}

	static void ObfuscateIfUnknown(int characterId, game::Island& island)
	{
		auto islandId = data::game::Island::Find(island.absoluteLocation).value();
		if (!data::game::character::KnownIsland::Read(characterId, islandId))
		{
			island.name = Islands::UNKNOWN;
		}
	}

	std::list<Island> Islands::GetKnownIslands(int characterId)
	{
		int shipId = game::characters::Ships::ReadShipId(characterId).value();
		auto shipLocation = game::Ship::GetLocation(shipId).value();
		auto knownLocations = data::game::character::KnownIsland::All(characterId);
		std::list<Island> result;
		for (auto& knownLocation : knownLocations)
		{
			auto model = Read(characterId, knownLocation);
			if (model)
			{
				model.value().relativeLocation = model.value().absoluteLocation - shipLocation;
				ObfuscateIfUnknown(characterId, model.value());
				result.push_back(model.value());
			}
		}
		return result;
	}

	void Islands::ApplyTurnEffects()
	{
	}
}
