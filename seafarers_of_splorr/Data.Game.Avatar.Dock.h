#pragma once
#include "Common.XY.h"
#include <optional>
namespace data::game::avatar::Dock
{
	void SetLocation(std::optional<common::XY<double>>);
	std::optional<common::XY<double>> GetLocation();
}