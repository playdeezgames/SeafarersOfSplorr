#pragma once
#include <Common.XY.h>
#include <optional>
namespace data::game::avatar
{
	class Destination
	{
	public:
		static void Write(int, const common::XY<double>&);
		static std::optional<common::XY<double>> Read(int);
		static void Clear(int);
		static void ClearAll();
	};
}