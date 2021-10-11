#pragma once
#include <Common.XY.h>
#include <optional>
namespace data::game::avatar
{
	struct Dock//20211011
	{
		static void Write(int, const common::XY<double>&);
		static void Clear(int);
		static std::optional<common::XY<double>> Read(int);
	};
}