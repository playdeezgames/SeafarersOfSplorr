#pragma once
#include <string>
#include "Common.XY.h"
namespace visuals::Images
{
	void SetSprite(const std::string&, const std::string&, const std::string&);
	void SetVisible(const std::string&, const std::string&, bool);
	void SetAngle(const std::string&, const std::string&, double);
	void SetLocation(const std::string&, const std::string&, const common::XY<int>&);

}
