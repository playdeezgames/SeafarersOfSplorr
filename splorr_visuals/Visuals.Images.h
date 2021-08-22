#pragma once
#include <Common.XY.h>
#include <json.hpp>
#include <string>
#include "Visuals.DrawerFunction.h"
namespace visuals::Images
{
	void SetSprite(const std::string&, const std::string&, const std::string&);
	void SetVisible(const std::string&, const std::string&, bool);
	void SetAngle(const std::string&, const std::string&, double);
	void SetLocation(const std::string&, const std::string&, const common::XY<int>&);

}
