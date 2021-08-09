#pragma once
#include "Common.XY.h"
#include <functional>
#include <string>
#include "Visuals.HorizontalAlignment.h"
namespace visuals::SpriteGrid
{
	void SetCell(const std::string&, const std::string&, const common::XY<int>&, const std::string&, const std::string&);
	void WriteText(const std::string&, const std::string&, const common::XY<int>&, const std::string&, const std::string&, const std::string&, const visuals::HorizontalAlignment&);
	void ClearCell(const std::string&, const std::string&, const common::XY<int>&);
	void Clear(const std::string&, const std::string&);
	int GetCellHeight(const std::string&, const std::string&);
	std::function<void(const common::XY<int>&, const std::string&, const std::string&)> DoWriteToGrid(const std::string&, const std::string&, const std::string&, const visuals::HorizontalAlignment&);
}