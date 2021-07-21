#pragma once
#include <string>
#include "Common.XY.h"
namespace visuals::SpriteGrid
{
	void SetCell(const std::string&, const std::string&, const common::XY<int>&, const std::string&, const std::string&);
	void WriteText(const std::string&, const std::string&, const common::XY<int>&, const std::string&, const std::string&, const std::string&);
	void ClearCell(const std::string&, const std::string&, const common::XY<int>&);
	void Clear(const std::string&, const std::string&);
}