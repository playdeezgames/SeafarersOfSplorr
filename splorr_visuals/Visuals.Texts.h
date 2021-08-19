#pragma once
#include <string>
#include "Common.XY.h"
#include "json.hpp"
#include "Visuals.DrawerFunction.h"
namespace visuals::Texts
{
	void SetText(const std::string&, const std::string&, const std::string&);
	void SetColor(const std::string&, const std::string&, const std::string&);
	void SetLocation(const std::string&, const std::string&, const common::XY<int>&);
}