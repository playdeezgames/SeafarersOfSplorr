#include "Common.XY.h"
#include <string>
#include <set>
#include <functional>
#include <optional>
#pragma once
namespace visuals
{
	struct AreaMenu
	{
		std::string menuId;
		std::string menuItemId;
	};
	struct Area
	{
		std::string areaId;
		common::XY<int> xy;
		common::XY<size_t> size;
		bool Contains(const common::XY<int>&) const;
		std::optional<AreaMenu> menu;
	};
}
namespace visuals::Areas
{
	std::set<std::string> Get(const std::string&, const common::XY<int>&);
	Area Get(const std::string&, const std::string&);
	std::function<void(const common::XY<int>&)> HandleMenuMouseMotion(const std::string&);
	std::function<void(const common::XY<int>&)> HandleMouseMotion(const std::string&, std::function<void(const std::string&, const common::XY<int>&)>, std::function<void(const common::XY<int>&)>);
	std::function<void(const common::XY<int>&)> HandleMouseMotion(const std::string&, std::function<void(const std::string&, const common::XY<int>&)>);
	std::function<bool(const common::XY<int>&, MouseButton)> HandleMenuMouseButtonUp(const std::string&, std::function<void()>);
	std::function<bool(const common::XY<int>&, MouseButton)> HandleMouseButtonUp(const std::string&, std::function<bool(const std::string&)>);
}
