#pragma once
#include <Common.XY.h>
#include <optional>
namespace data::game::avatar::Dock
{
	void SetLocation(const common::XY<double>&);
	void ClearLocation();
	std::optional<common::XY<double>> GetLocation();
}