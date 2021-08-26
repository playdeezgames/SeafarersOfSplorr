#include <Common.XY.h>
#include <functional>
#include <Game.Avatar.AtSea.h>
#include <Game.Colors.h>
#include <Game.Merchants.h>
#include <Game.World.h>
#include <Visuals.Colors.h>
#include <Visuals.Data.Properties.h>
#include "Visuals.Navigator.h"
#include <Visuals.Sprites.h>
namespace visuals
{
	struct InternalNavigator
	{
		common::XY<int> xy;
		size_t radius;
	};

	static std::vector<InternalNavigator> navigators;
	static std::map<std::string, std::map<std::string, size_t>> navigatorTable;

	static std::function<common::XY<int>(const common::XY<double>&)> MakePlotter(const InternalNavigator& navigator)
	{
		return [navigator](const common::XY<double>& location)
		{
			auto avatarLocation = game::avatar::AtSea::GetLocation();
			auto difference = (location - avatarLocation);
			auto scaledDifference = difference * (double)navigator.radius / game::World::GetViewDistance();
			common::XY<int> plottedDifference = {(int)scaledDifference.GetX(), (int)scaledDifference.GetY()};
			return common::XY<int>(plottedDifference.GetX()+navigator.xy.GetX(), plottedDifference.GetY() + navigator.xy.GetY());
		};
	}

	static std::function<bool(const common::XY<int>&)> MakeDrawFilter(const InternalNavigator& navigator)
	{
		return [navigator](const common::XY<int>& plot)
		{
			auto difference = plot - navigator.xy;
			return difference.GetMagnitude() < navigator.radius;
		};
	}

	static void DrawMerchants(
		const std::shared_ptr<application::Engine::Renderer>& renderer,
		std::function<common::XY<int>(const common::XY<double>&)> plotter,
		std::function<bool(const common::XY<int>&)> drawFilter)
	{
		auto merchants = game::Merchants::All();
		for (auto& merchant : merchants)
		{
			auto plot = plotter(merchant.location);
			if (drawFilter(plot))
			{
				Sprites::Draw("AtSeaMerchantShip", renderer, plot, visuals::Colors::Read(game::Colors::WHITE));
			}
		}
	}

	static void DrawInternalNavigator(const std::shared_ptr<application::Engine::Renderer>& renderer, size_t navigatorIndex)
	{
		auto& navigator = navigators[navigatorIndex];
		DrawMerchants(renderer, MakePlotter(navigator), MakeDrawFilter(navigator));
	}

	std::function<void(const std::shared_ptr<application::Engine::Renderer>&)> Navigator::Internalize(const std::string& layoutName, const nlohmann::json& model)
	{
		auto navigatorIndex = navigators.size();
		navigators.push_back({
			common::XY<int>(model[visuals::data::Properties::X], model[visuals::data::Properties::Y]),
			model[visuals::data::Properties::RADIUS]
			});
		if (model.count(visuals::data::Properties::WORLD_MAP_ID) > 0)
		{
			navigatorTable[layoutName][model[visuals::data::Properties::WORLD_MAP_ID]] = navigatorIndex;
		}

		return [navigatorIndex](const std::shared_ptr<application::Engine::Renderer>& renderer)
		{
			DrawInternalNavigator(renderer, navigatorIndex);
		};
	}
}
