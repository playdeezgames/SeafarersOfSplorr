#pragma once
#include <Common.XY.h>
#include <optional>
namespace data::game::avatar
{
	struct Dock
	{
		static void Write(int, const common::XY<double>&);
		static void Clear(int);
		static std::optional<common::XY<double>> Read(int);
		static void Write(const common::XY<double>&);
		static void Clear();
		static std::optional<common::XY<double>> Read();
	};
}