#include "Application.MouseButtonUp.h"
#include "Common.Application.h"
#include "Common.Data.Properties.h"
#include "json.hpp"
#include <SDL.h>
#include "Visuals.Areas.h"
#include "Visuals.Data.Types.h"
#include "Visuals.Data.Properties.h"
#include "Visuals.Menus.h"
namespace visuals::Layouts
{
	nlohmann::json& GetLayout(const std::string&);
}
namespace visuals
{
	bool Area::Contains(const common::XY<int>& location) const
	{
		auto difference = location - xy;
		return(difference.GetX() >= 0 && difference.GetY() >= 0 && difference.GetX() < size.GetX() && difference.GetY() < size.GetY());
	}
}
namespace visuals::Areas
{
	static std::map<std::string, std::vector<visuals::Area>> areaLists;
	static std::map<std::string, std::map<std::string, visuals::Area>> areaTable;

	std::function<void(const std::shared_ptr<common::Application::Renderer>&)> Internalize(const std::string& layoutName, const nlohmann::json& model)
	{
		int x = model[common::data::Properties::X];
		int y = model[common::data::Properties::Y];
		size_t width = model[common::data::Properties::WIDTH];
		size_t height = model[common::data::Properties::HEIGHT];
		std::string areaId = model[visuals::data::Properties::AREA_ID];
		std::optional<AreaMenu> areaMenu = std::nullopt;
		if (model.count(visuals::data::Properties::MENU))
		{
			areaMenu = 
				{
					model[visuals::data::Properties::MENU][visuals::data::Properties::MENU_ID],
					model[visuals::data::Properties::MENU][visuals::data::Properties::MENU_ITEM_ID]
				};
		}
		visuals::Area area =
		{
			areaId,
			common::XY<int>(x,y),
			common::XY<size_t>(width, height),
			areaMenu
		};
		areaLists[layoutName].push_back(area);
		areaTable[layoutName][areaId] = area;
		return [](const std::shared_ptr<common::Application::Renderer>&) {};
	}

	static const std::vector<visuals::Area>& GetForLayout(const std::string& layoutName)
	{
		return areaLists.find(layoutName)->second;
	}

	std::set<std::string> Get(const std::string& layoutName, const common::XY<int>& xy)
	{
		std::set<std::string> result;
		for (auto& area : GetForLayout(layoutName))
		{
			if (area.Contains(xy))
			{
				result.insert(area.areaId);
			}
		}
		return result;
	}

	Area Get(const std::string& layoutName, const std::string& areaName)
	{
		return areaTable.find(layoutName)->second.find(areaName)->second;
	}

	std::function<void(const common::XY<int>& xy)> 
		HandleMouseMotion(
			const std::string& layoutName, 
			std::function<void(const std::string&, const common::XY<int>&)> areaHandler,
			std::function<void(const common::XY<int>&)> noAreaHandler
		)
	{
		return [layoutName, areaHandler, noAreaHandler](const common::XY<int>& xy)
		{
			auto areas = visuals::Areas::Get(layoutName, xy);
			if (areas.empty())
			{
				noAreaHandler(xy);
				return;
			}
			for (auto& area : areas)
			{
				auto a = Get(layoutName, area);
				areaHandler(area, xy - a.xy);
			}
		};
	}

	std::function<void(const common::XY<int>& xy)>
		HandleMouseMotion(
			const std::string& layoutName,
			std::function<void(const std::string&, const common::XY<int>&)> areaHandler
		)
	{
		return HandleMouseMotion(layoutName, areaHandler, [](const common::XY<int>&) {});
	}


	std::function<bool(const common::XY<int>&, MouseButton)> HandleMouseButtonUp(const std::string& layoutName, std::function<bool(const std::string&)> areaHandler)
	{
		return [layoutName, areaHandler](const common::XY<int>& xy, MouseButton)
		{
			auto areas = visuals::Areas::Get(layoutName, xy);
			for (auto& area : areas)
			{
				if (areaHandler(area))
				{
					return true;
				}
			}
			return false;
		};
	}

	std::function<void(const common::XY<int>&)> HandleMenuMouseMotion(const std::string& layoutName)
	{
		return [layoutName](const common::XY<int>& xy) {
			auto areas = visuals::Areas::Get(layoutName, xy);
			if (!areas.empty())
			{
				for (auto& area : areas)
				{
					auto a = Get(layoutName, area);
					if (a.menu)
					{
						visuals::Menus::WriteMenuItemId(layoutName, a.menu.value().menuId, a.menu.value().menuItemId);
					}
				}
			}
		};
	}

	std::function<bool(const common::XY<int>&, MouseButton)> HandleMenuMouseButtonUp(const std::string& layoutName, std::function<void()> handler)
	{
		return [layoutName, handler](const common::XY<int>& xy, MouseButton)
		{
			auto areas = visuals::Areas::Get(layoutName, xy);
			if (!areas.empty())
			{
				for (auto& area : areas)
				{
					auto a = Get(layoutName, area);
					if (a.menu)
					{
						visuals::Menus::WriteMenuItemId(layoutName, a.menu.value().menuId, a.menu.value().menuItemId);
						handler();
						return true;
					}
				}
			}
			return false;
		};
	}
}