#pragma once
#include <Common.XY.h>
#include <functional>
#include <json.hpp>
#include <MouseButton.h>
#include <optional>
#include <set>
#include <string>
#include "Visuals.Area.h"
#include "Visuals.DrawerFunction.h"
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
