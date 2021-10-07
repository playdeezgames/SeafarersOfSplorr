#pragma once
#include <Common.XY.h>
#include <optional>
namespace data::game::avatar
{
	struct Destination
	{
		static void Write(int, int, const common::XY<double>&);
		static std::optional<common::XY<double>> Read(int, int);
		static void Clear(int, int);
		static void ClearAll(int);
	};
}