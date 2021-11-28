#include <Common.Heading.h>
#include <Common.RNG.h>
#include <Data.Game.Island.Item.h>
#include <Data.Game.Island.h>
#include <Data.Game.Island.Known.h>
#include <Data.Game.Island.Market.h>
#include <Data.Game.Island.Quest.h>
#include <Data.Game.Island.Visit.h>
#include <functional>
#include "Game.Character.Ship.h"
#include "Game.Commodities.h"
#include "Game.Islands.h"
#include "Game.Items.h"
#include "Game.Ship.h"
#include "Game.Player.h"
#include "Game.World.h"
#include <map>
#include <set>
#include <sstream>
#include <vector>
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
		const common::XY<double>& avatarLocation)
	{
		auto visitData = data::game::island::Visit::Read(island.id);
		data::game::island::Known::Write(island.id);
		accumulator.push_back(
			{
				island.id,
				(island.location - avatarLocation),
				island.location,
				island.name,
				(visitData.has_value()) ? (std::optional<int>(visitData.value().visits)) : (std::nullopt),
				island.patronDemigodId
			});
	}

	static void KnowIslandWhenCloseEnough(std::list<Island>& accumulator, const data::game::Island& island, const common::XY<double>& avatarLocation, std::function<bool(const data::game::Island&, double)> filter)
	{
		auto distance = common::Heading::Distance(avatarLocation, island.location);
		if (filter(island, distance))
		{
			KnowIsland(accumulator, island, avatarLocation);
		}
	}

	static std::list<Island> GetIslandsInRange(std::function<bool(const data::game::Island&, double)> filter)
	{
		std::list<Island> accumulator;
		auto avatarLocation = game::Ship::GetLocation(game::character::Ship::ReadShipId(game::Player::GetCharacterId()).value()).value();;
		auto islands = data::game::Island::All();
		for (auto& island : islands)
		{
			KnowIslandWhenCloseEnough(accumulator, island, avatarLocation, filter);
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

	std::list<Island> Islands::GetViewableIslands()
	{
		return GetIslandsInRange(FixedDistance(game::World::GetViewDistance()));
	}

	std::list<Island> Islands::GetDockableIslands()
	{
		return GetIslandsInRange(FixedDistance(game::World::GetDockDistance()));
	}

	bool Islands::CanDock()
	{
		return !GetDockableIslands().empty();
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

	void Islands::AddVisit(const common::XY<double>& location, const int& currentTurn)
	{
		auto islandId = data::game::Island::Find(location).value();
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

	void Islands::SetKnown(const common::XY<double>& location, const int& turn)
	{
		auto islandId = data::game::Island::Find(location).value();
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

	std::optional<Island> Islands::Read(const common::XY<double>& location)
	{
		auto islandId = data::game::Island::Find(location).value();
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

	std::list<Island> Islands::GetKnownIslands(const common::XY<double>& avatarLocation)
	{
		auto knownLocations = data::game::island::Known::All();
		std::list<Island> result;
		for (auto& knownLocation : knownLocations)
		{
			auto model = Read(data::game::Island::Read(knownLocation).value().location);
			if (model)
			{
				model.value().relativeLocation = model.value().absoluteLocation - avatarLocation;
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
