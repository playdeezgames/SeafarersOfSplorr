#include "Visuals.WorldMap.h"
#include <functional>
#include "json.hpp"
#include "Common.Application.h"
#include "Common.XY.h"
#include "Common.Data.Properties.h"
#include "Game.World.h"
#include "Game.Avatar.h"
#include "Visuals.Sprites.h"
#include "Game.Islands.h"
namespace visuals::WorldMap
{
	struct InternalWorldMap
	{
		common::XY<int> xy;
		common::XY<size_t> size;
	};

	static std::vector<InternalWorldMap> worldMaps;

	static common::XY<int> Plot(const InternalWorldMap& worldMap, const common::XY<double> worldSize, const common::XY<double>& location)
	{
		return common::XY<int>(
			location.GetX() / worldSize.GetX() * worldMap.size.GetX() + worldMap.xy.GetX(),
			location.GetY() / worldSize.GetY() * worldMap.size.GetY() + worldMap.xy.GetY());
	}

	static void DrawAvatar(const std::shared_ptr<common::Application::Renderer>& renderer, const InternalWorldMap& worldMap, const common::XY<double> worldSize)
	{
		auto avatarLocation = game::Avatar::GetLocation();
		common::XY<int> plot = Plot(worldMap, worldSize, avatarLocation);
		visuals::Sprites::Draw("WorldMapShip", renderer, plot, { 0xff,0xff,0xff,0xff });
	}

	static void DrawKnownIslands(const std::shared_ptr<common::Application::Renderer>& renderer, const InternalWorldMap& worldMap, const common::XY<double> worldSize)
	{
		auto knownIslands = game::Islands::GetKnownIslands();
		for (auto& knownIsland : knownIslands)
		{
			common::XY<int> plot = Plot(worldMap, worldSize, knownIsland.absoluteLocation);
			visuals::Sprites::Draw("WorldMapIsland", renderer, plot, { 0xff,0xff,0xff,0xff });
		}
	}

	static void DrawInternalWorldMap(const std::shared_ptr<common::Application::Renderer>& renderer, size_t worldMapIndex)
	{
		auto& worldMap = worldMaps[worldMapIndex];
		auto worldSize = game::World::GetSize();
		DrawKnownIslands(renderer, worldMap, worldSize);
		DrawAvatar(renderer, worldMap, worldSize);


	}

	std::function<void(const std::shared_ptr<common::Application::Renderer>&)> Internalize(const std::string& layoutName, const nlohmann::json& model)
	{
		auto worldMapIndex = worldMaps.size();
		worldMaps.push_back({
			common::XY<int>(model[common::data::Properties::X], model[common::data::Properties::Y]),
			common::XY<size_t>(model[common::data::Properties::WIDTH], model[common::data::Properties::HEIGHT])
			});

		return [worldMapIndex](const std::shared_ptr<common::Application::Renderer>& renderer)
		{
			DrawInternalWorldMap(renderer, worldMapIndex);
		};
	}
}
