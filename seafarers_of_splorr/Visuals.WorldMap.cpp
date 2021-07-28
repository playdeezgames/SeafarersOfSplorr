#include "Common.Application.h"
#include "Common.Data.Properties.h"
#include "Common.XY.h"
#include <functional>
#include "Game.Avatar.h"
#include "Game.Avatar.Destination.h"
#include "Game.Avatar.Quest.h"
#include "Game.Heading.h"
#include "Game.Islands.h"
#include "Game.World.h"
#include "json.hpp"
#include <map>
#include "Visuals.Data.Properties.h"
#include "Visuals.Sprites.h"
#include "Visuals.WorldMap.h"
namespace visuals::WorldMap
{
	const std::string SPRITE_WORLD_MAP_SHIP = "WorldMapShip";
	const std::string SPRITE_WORLD_MAP_NEW_DESTINATION = "WorldMapNewDestination";
	const std::string SPRITE_WORLD_MAP_CURRENT_DESTINATION_1 = "WorldMapCurrentDestination1";
	const std::string SPRITE_WORLD_MAP_CURRENT_DESTINATION_2 = "WorldMapCurrentDestination2";
	const std::string SPRITE_WORLD_MAP_CURRENT_DESTINATION_3 = "WorldMapCurrentDestination3";
	const std::string SPRITE_WORLD_MAP_CURRENT_DESTINATION_4 = "WorldMapCurrentDestination4";
	const std::string SPRITE_WORLD_MAP_QUEST_ISLAND = "WorldMapQuestIsland";
	const std::string SPRITE_WORLD_MAP_ISLAND = "WorldMapIsland";
	const std::string SPRITE_WORLD_MAP_HOVER_ISLAND = "WorldMapHoverIsland";

	struct InternalWorldMap
	{
		common::XY<int> xy;
		common::XY<size_t> size;
		std::optional<common::XY<int>> destination;
		std::optional<common::XY<double>> hoverIsland;
	};

	static std::vector<InternalWorldMap> worldMaps;
	static std::map<std::string, std::map<std::string, size_t>> worldMapTable;

	static common::XY<int> Plot(const InternalWorldMap& worldMap, const common::XY<double> worldSize, const common::XY<double>& location)
	{
		return common::XY<int>(
			(int)(location.GetX() / worldSize.GetX() * worldMap.size.GetX() + worldMap.xy.GetX()),
			(int)(location.GetY() / worldSize.GetY() * worldMap.size.GetY() + worldMap.xy.GetY()));
	}

	static common::XY<double> Pick(const InternalWorldMap& worldMap, const common::XY<double> worldSize, const common::XY<int>& location)
	{
		return common::XY<double>(
			location.GetX() * worldSize.GetX() / worldMap.size.GetX(),
			location.GetY() * worldSize.GetY() / worldMap.size.GetY());
	}

	static void DrawAvatar(const std::shared_ptr<common::Application::Renderer>& renderer, const InternalWorldMap& worldMap, const common::XY<double> worldSize)
	{
		auto avatarLocation = game::Avatar::GetLocation();
		common::XY<int> plot = Plot(worldMap, worldSize, avatarLocation);
		visuals::Sprites::Draw(SPRITE_WORLD_MAP_SHIP, renderer, plot, { 0xff,0xff,0xff,0xff });
	}

	static void DrawNewDestination(const std::shared_ptr<common::Application::Renderer>& renderer, const InternalWorldMap& worldMap, const common::XY<double> worldSize)
	{
		auto newDestination = worldMap.destination;
		if (newDestination)
		{
			visuals::Sprites::Draw(SPRITE_WORLD_MAP_NEW_DESTINATION, renderer, newDestination.value() + worldMap.xy, { 0xff,0xff,0xff,0xff });
		}
	}

	static void DrawCurrentDestination(const std::shared_ptr<common::Application::Renderer>& renderer, const InternalWorldMap& worldMap, const common::XY<double> worldSize)
	{
		//TODO: select destination number
		auto currentDestination = game::avatar::destination::GetDestination(game::avatar::Destination::ONE);
		if (currentDestination)
		{
			common::XY<int> plot = Plot(worldMap, worldSize, currentDestination.value());
			visuals::Sprites::Draw(SPRITE_WORLD_MAP_CURRENT_DESTINATION_1, renderer, plot, { 0xff,0xff,0xff,0xff });
		}
	}

	static void DrawKnownIslands(const std::shared_ptr<common::Application::Renderer>& renderer, InternalWorldMap& worldMap, const common::XY<double> worldSize)
	{
		auto closest = game::Heading::Distance({(double)worldMap.size.GetX(), (double)worldMap.size.GetY()}, {0.0,0.0});
		std::optional<common::XY<int>> hoverIsland = std::nullopt;
		auto knownIslands = game::Islands::GetKnownIslands();
		worldMap.hoverIsland = std::nullopt;
		auto quest = game::avatar::Quest::Read();
		for (auto& knownIsland : knownIslands)
		{
			common::XY<int> plot = Plot(worldMap, worldSize, knownIsland.absoluteLocation);
			if (worldMap.destination)
			{
				auto distance = game::Heading::Distance({ (double)plot.GetX(),(double)plot.GetY() }, { (double)worldMap.destination.value().GetX() + worldMap.xy.GetX(), (double)worldMap.destination.value().GetY() + worldMap.xy.GetY()});
				if (distance < closest)
				{
					closest = distance;
					hoverIsland = plot;
					worldMap.hoverIsland = knownIsland.absoluteLocation;
				}
			}
			if (quest.has_value() && quest.value().destination == knownIsland.absoluteLocation)
			{
				visuals::Sprites::Draw(SPRITE_WORLD_MAP_QUEST_ISLAND, renderer, plot, { 0xff,0xff,0xff,0xff });
			}
			else
			{
				visuals::Sprites::Draw(SPRITE_WORLD_MAP_ISLAND, renderer, plot, { 0xff,0xff,0xff,0xff });
			}
		}
		if (hoverIsland)
		{
			visuals::Sprites::Draw(SPRITE_WORLD_MAP_HOVER_ISLAND, renderer, hoverIsland.value(), { 0xff,0xff,0xff,0xff });
		}
	}

	static void DrawInternalWorldMap(const std::shared_ptr<common::Application::Renderer>& renderer, size_t worldMapIndex)
	{
		auto& worldMap = worldMaps[worldMapIndex];
		auto worldSize = game::World::GetSize();
		DrawKnownIslands(renderer, worldMap, worldSize);
		DrawAvatar(renderer, worldMap, worldSize);
		DrawCurrentDestination(renderer, worldMap, worldSize);
		DrawNewDestination(renderer, worldMap, worldSize);
	}

	std::function<void(const std::shared_ptr<common::Application::Renderer>&)> Internalize(const std::string& layoutName, const nlohmann::json& model)
	{
		auto worldMapIndex = worldMaps.size();
		worldMaps.push_back({
			common::XY<int>(model[common::data::Properties::X], model[common::data::Properties::Y]),
			common::XY<size_t>(model[common::data::Properties::WIDTH], model[common::data::Properties::HEIGHT]),
			std::nullopt
			});
		if (model.count(visuals::data::Properties::WORLD_MAP_ID)>0)
		{
			worldMapTable[layoutName][model[visuals::data::Properties::WORLD_MAP_ID]] = worldMapIndex;
		}

		return [worldMapIndex](const std::shared_ptr<common::Application::Renderer>& renderer)
		{
			DrawInternalWorldMap(renderer, worldMapIndex);
		};
	}

	void SetDestination(const std::string& layoutName, const std::string& worldMapId, const std::optional<common::XY<int>>& destination)
	{
		size_t index = worldMapTable.find(layoutName)->second.find(worldMapId)->second;
		worldMaps[index].destination = destination;

	}

	std::optional<common::XY<double>> GetDestination(const std::string& layoutName, const std::string& worldMapId)
	{
		auto layoutIter = worldMapTable.find(layoutName);
		if (layoutIter != worldMapTable.end())
		{
			auto worldMapIter = layoutIter->second.find(worldMapId);
			if (worldMapIter != layoutIter->second.end())
			{
				size_t index = worldMapIter->second;
				auto worldMap = worldMaps[index];
				if (worldMap.destination)
				{
					return Pick(worldMaps[index], game::World::GetSize(), worldMap.destination.value());
				}
			}
		}
		return std::nullopt;
	}

	std::optional<common::XY<double>> GetHoverIsland(const std::string& layoutName, const std::string& worldMapId)
	{
		auto layoutIter = worldMapTable.find(layoutName);
		if (layoutIter != worldMapTable.end())
		{
			auto worldMapIter = layoutIter->second.find(worldMapId);
			if (worldMapIter != layoutIter->second.end())
			{
				size_t index = worldMapIter->second;
				const auto& worldMap = worldMaps[index];
				return worldMap.hoverIsland;
			}
		}
		return std::nullopt;
	}

}
