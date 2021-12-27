#pragma once
#include <optional>
#include <list>
#include <tuple>
namespace data::game::character
{
	struct DemigodFavor
	{
		static void Initialize();
		static void Write(int, int, double, int);
		static std::optional<double> ReadFavor(int, int);
		static std::optional<int> ReadOfferingCooldown(int, int);
		static void WriteOfferingCooldown(int, int, int);
		static std::list<std::tuple<int, int, int>> ReadOfferingCooldowns();
		static void WriteOfferingCooldowns(const std::list<std::tuple<int, int, int>>&);
		static void ClearAll();
	};
}
