#pragma once
#include <optional>
namespace data::game::character
{
	struct DemigodFavor
	{
		static void Write(int, int, double, int);
		static std::optional<double> ReadFavor(int, int);
		static std::optional<int> ReadOfferingCooldown(int, int);
		static void Clear(int);
	};
}
