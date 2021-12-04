#include <Common.Heading.h>
#include <Data.Game.Island.h>
#include <Data.Game.Island.Known.h>
#include <Data.Game.Island.Visit.h>
#include "Game.Characters.Ships.h"
#include "Game.Islands.h"
#include "Game.Ship.h"
#include "Game.World.h"
namespace game
{
	const std::string Islands::UNKNOWN = "????";

	void GenerateIslands();

	void Islands::Reset(const game::Difficulty&)
	{
		GenerateIslands();
	}

	static void KnowIsland(
		std::list<Island>& accumulator, 
		const data::game::Island& island, 
		const common::XY<double>& shipLocation)
	{
		auto visitData = data::game::island::Visit::Read(island.id);
		data::game::island::Known::Write(island.id);
		accumulator.push_back(
			{
				island.id,
				(island.location - shipLocation),
				island.location,
				island.name,
				(visitData.has_value()) ? (std::optional<int>(visitData.value().visits)) : (std::nullopt),
				island.patronDemigodId
			});
	}

	static void KnowIslandWhenCloseEnough(std::list<Island>& accumulator, const data::game::Island& island, const common::XY<double>& shipLocation, std::function<bool(const data::game::Island&, double)> filter)
	{
		auto distance = common::Heading::Distance(shipLocation, island.location);
		if (filter(island, distance))
		{
			KnowIsland(accumulator, island, shipLocation);
		}
	}

	static std::list<Island> GetIslandsInRange(int shipId, std::function<bool(const data::game::Island&, double)> filter)
	{
		std::list<Island> accumulator;
		auto shipLocation = game::Ship::GetLocation(shipId).value();;
		auto islands = data::game::Island::All();
		for (auto& island : islands)
		{
			KnowIslandWhenCloseEnough(accumulator, island, shipLocation, filter);
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
		return GetIslandsInRange(shipId, FixedDistance(game::World::GetViewDistance()));
	}

	std::list<Island> Islands::GetDockableIslands(int shipId)
	{
		return GetIslandsInRange(shipId, FixedDistance(game::World::GetDockDistance()));
	}

	bool Islands::CanDock(int shipId)
	{
		return !GetDockableIslands(shipId).empty();
	}

	static void AddSubsequentVisit(
		data::game::island::Visit islandVisits, 
		const int& currentTurn)
	{
		if (islandVisits.lastVisit != currentTurn)
		{
			islandVisits.visits++;
			islandVisits.lastVisit = currentTurn;
			data::game::island::Visit::Write(islandVisits);
		}
	}

	static void AddInitialVisit(int islandId, const int& currentTurn)
	{
		data::game::island::Visit::Write({
			islandId,
			1,
			currentTurn });
	}

	void Islands::AddVisit(int islandId, int currentTurn)
	{
		auto previousVisits = data::game::island::Visit::Read(islandId);
		if (previousVisits)
		{
			AddSubsequentVisit(previousVisits.value(), currentTurn);
		}
		else
		{
			AddInitialVisit(islandId, currentTurn);
		}
	}

	void Islands::SetKnown(int islandId, int turn)
	{
		if (!data::game::island::Visit::Read(islandId))
		{
			data::game::island::Visit::Write({
				islandId,
				0,
				turn });
		}
	}

	static Island ToIsland(const data::game::Island& island)
	{
		auto previousVisits = data::game::island::Visit::Read(island.id);
		return 
		{
			island.id,
			{0.0, 0.0},
			island.location,
			island.name,
			(previousVisits.has_value()) ? (std::optional<int>(previousVisits.value().visits)) : (std::nullopt),
			island.patronDemigodId
		};
	}

	std::optional<Island> Islands::Read(int islandId)
	{
		auto island = data::game::Island::Read(islandId);
		if (island)
		{
			return ToIsland(island.value());
		}
		return std::nullopt;
	}

	static void ObfuscateIfUnknown(game::Island& island)
	{
		auto islandId = data::game::Island::Find(island.absoluteLocation).value();
		if (!data::game::island::Visit::Read(islandId))
		{
			island.name = Islands::UNKNOWN;
		}
	}

	std::list<Island> Islands::GetKnownIslands(int characterId)
	{
		int shipId = game::characters::Ships::ReadShipId(characterId).value();
		auto shipLocation = game::Ship::GetLocation(shipId).value();
		auto knownLocations = data::game::island::Known::All();
		std::list<Island> result;
		for (auto& knownLocation : knownLocations)
		{
			auto model = Read(knownLocation);
			if (model)
			{
				model.value().relativeLocation = model.value().absoluteLocation - shipLocation;
				ObfuscateIfUnknown(model.value());
				result.push_back(model.value());
			}
		}
		return result;
	}

	void Islands::ApplyTurnEffects()
	{
	}
}
