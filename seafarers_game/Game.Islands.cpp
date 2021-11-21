#include <Common.Heading.h>
#include <Common.RNG.h>
#include <Data.Game.Island.Item.h>
#include <Data.Game.Island.h>
#include <Data.Game.Island.Known.h>
#include <Data.Game.Island.Market.h>
#include <Data.Game.Island.Quest.h>
#include <Data.Game.Island.Visit.h>
#include <functional>
#include <Game.Avatar.Ship.h>
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
		auto visitData = data::game::island::Visit::Read(island.location);
		data::game::island::Known::Write(island.location);
		accumulator.push_back(
			{
				(island.location - avatarLocation),
				island.location,
				island.name,
				(visitData.has_value()) ? (std::optional<int>(visitData.value().visits)) : (std::nullopt),
				island.patronDemigod
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
		auto avatarLocation = game::Ship::GetLocation(game::avatar::Ship::ReadShipId(game::Player::GetAvatarId()).value()).value();;
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

	static void AddInitialVisit(const common::XY<double>& location, const int& currentTurn)
	{
		data::game::island::Visit::Write({
			location,
			1,
			currentTurn });
	}

	void Islands::AddVisit(const common::XY<double>& location, const int& currentTurn)
	{
		auto previousVisits = data::game::island::Visit::Read(location);
		if (previousVisits)
		{
			AddSubsequentVisit(previousVisits.value(), currentTurn);
		}
		else
		{
			AddInitialVisit(location, currentTurn);
		}
	}

	void Islands::SetKnown(const common::XY<double>& location, const int& turn)
	{
		if (!data::game::island::Visit::Read(location))
		{
			data::game::island::Visit::Write({
				location,
				0,
				turn });
		}
	}

	static Island ToIsland(const data::game::Island& island)
	{
		auto previousVisits = data::game::island::Visit::Read(island.location);
		return {
				{0.0, 0.0},
				island.location,
				island.name,
				(previousVisits.has_value()) ? (std::optional<int>(previousVisits.value().visits)) : (std::nullopt),
				island.patronDemigod
			};
	}

	std::optional<Island> Islands::Read(const common::XY<double>& location)
	{
		auto island = data::game::Island::Read(location);
		if (island)
		{
			return ToIsland(island.value());
		}
		return std::nullopt;
	}

	static void ObfuscateIfUnknown(game::Island& island)
	{
		if (!data::game::island::Visit::Read(island.absoluteLocation))
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
			auto model = Read(knownLocation);
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
