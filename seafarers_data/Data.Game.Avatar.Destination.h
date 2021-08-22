#pragma once
#include <Common.XY.h>
#include <optional>
namespace data::game::avatar::Destination
{
	void Write(int, const common::XY<double>&);
	std::optional<common::XY<double>> Read(int);
	void Clear(int);
	void ClearAll();
}