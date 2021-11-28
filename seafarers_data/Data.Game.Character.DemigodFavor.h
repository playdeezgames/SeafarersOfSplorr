#pragma once
#include <optional>
namespace data::game::character
{
	struct DemigodFavor
	{
		static void Write(int, int, double);
		static std::optional<double> Read(int, int);
		static void Clear(int);
	};
}
