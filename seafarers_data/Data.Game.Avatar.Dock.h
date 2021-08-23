#pragma once
#include <Common.XY.h>
#include <optional>
namespace data::game::avatar
{
	struct Dock
	{
		static void SetLocation(const common::XY<double>&);
		static void ClearLocation();
		static std::optional<common::XY<double>> GetLocation();
	};
}