#pragma once
#include <optional>
#include "Common.XY.h"
namespace data::game::avatar::Destination
{
	void Write(const common::XY<double>&);
	std::optional<common::XY<double>> Read();
	void Clear();
}