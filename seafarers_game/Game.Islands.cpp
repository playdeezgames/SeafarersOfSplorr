#include <Common.Heading.h>
#include <Common.RNG.h>
#include <Data.Game.Avatar.Destination.h>
#include <Data.Game.Island.Item.h>
#include <Data.Game.Island.h>
#include <Data.Game.Island.Known.h>
#include <Data.Game.Island.Market.h>
#include <Data.Game.Island.Quests.h>
#include <Data.Game.Island.Visits.h>
#include <functional>
#include "Game.Avatar.AtSea.h"
#include "Game.Commodities.h"
#include "Game.Islands.h"
#include "Game.Items.h"
#include "Game.World.h"
#include <map>
#include <set>
#include <sstream>
#include <vector>
namespace game::Islands
{
	void GenerateIslands();

	void Reset(const game::Difficulty&)
	{
		GenerateIslands();
	}

	static void AddIsland(std::list<IslandModel>& result, const data::game::Island& island, const common::XY<double>& avatarLocation)
	{
		auto visitData = data::game::island::Visits::Read(island.location);
		data::game::island::Known::Write(island.location);
		result.push_back(
			{
				(island.location - avatarLocation),
				island.location,
				island.name,
				(visitData.has_value()) ? (std::optional<int>(visitData.value().visits)) : (std::nullopt)
			});
	}

	static void AddIslandWhenCloseEnough(std::list<IslandModel>& result, const data::game::Island& island, const common::XY<double>& avatarLocation, std::function<bool(const data::game::Island&, double)> filter)
	{
		auto distance = common::Heading::Distance(avatarLocation, island.location);
		if (filter(island, distance))
		{
			AddIsland(result, island, avatarLocation);
		}
	}

	static std::list<IslandModel> GetIslandsInRange(std::function<bool(const data::game::Island&, double)> filter)
	{
		std::list<IslandModel> result;
		auto avatarLocation = game::avatar::AtSea::GetLocation();
		auto islands = data::game::Island::All();
		for (auto& island : islands)
		{
			AddIslandWhenCloseEnough(result, island, avatarLocation, filter);
		}
		return result;
	}

	static std::function<bool(const data::game::Island&, double)> FixedDistance(double maximumDistance)
	{
		return [maximumDistance](const data::game::Island&, double distance)
		{
			return distance <= maximumDistance;
		};
	}

	std::list<IslandModel> GetViewableIslands()
	{
		return GetIslandsInRange(FixedDistance(game::World::GetViewDistance()));
	}

	std::list<IslandModel> GetDockableIslands()
	{
		return GetIslandsInRange(FixedDistance(game::World::GetDockDistance()));
	}

	std::list<IslandModel> GetCareeningIslands()
	{
		return GetIslandsInRange(
			[](const data::game::Island& data, double distance) 
			{
				return distance < data.careeningDistance;
			});
	}

	bool CanDock()
	{
		return !GetDockableIslands().empty();
	}

	bool CanCareen()
	{
		return !GetCareeningIslands().empty();
	}

	void AddVisit(const common::XY<double>& location, const int& turn)
	{
		auto visitData = data::game::island::Visits::Read(location);
		if (visitData)
		{
			auto islandVisits = visitData.value();
			if (islandVisits.lastVisit != turn)
			{
				islandVisits.visits = islandVisits.visits + 1;
				islandVisits.lastVisit = turn;
				data::game::island::Visits::Write(islandVisits);
				return;
			}
		}
		data::game::island::Visits::Write({
			location,
			1,
			turn});
	}

	void SetKnown(const common::XY<double>& location, const int& turn)
	{
		if (!data::game::island::Visits::Read(location))
		{
			data::game::island::Visits::Write({
				location,
				0,
				turn });
		}
	}

	std::optional<IslandModel> Read(const common::XY<double>& location)//TODO: different model for docked?
	{
		auto data = data::game::Island::Read(location);
		if (data)
		{
			auto visitData = data::game::island::Visits::Read(data.value().location);
			return std::optional<IslandModel>({
				{0.0, 0.0},
				data.value().location,
				data.value().name,
				(visitData.has_value()) ? (std::optional<int>(visitData.value().visits)) : (std::nullopt)
				});
		}
		return std::nullopt;
	}

	std::list<IslandModel> GetKnownIslands()
	{
		auto knownLocations = data::game::island::Known::All();
		std::list<IslandModel> result;
		for (auto& knownLocation : knownLocations)
		{
			auto model = Read(knownLocation);
			if (model)
			{
				auto visits = data::game::island::Visits::Read(knownLocation);
				if (visits)
				{
					result.push_back(model.value());
				}
				else
				{
					auto obfuscated = model.value();
					obfuscated.name = game::Islands::UNKNOWN;
					result.push_back(obfuscated);
				}
			}
		}
		return result;
	}
}
