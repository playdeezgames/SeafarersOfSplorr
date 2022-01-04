#pragma once
#include <vector>
#include <optional>
namespace data::game::item::type
{
	struct Commodity
	{
		Commodity() = delete;
		static void Initialize();
		static void Write(int itemTypeId, int commodity, double amount);
		static std::optional<double> ReadAmount(int itemTypeId, int commodity);
		static std::vector<int> AllCommodities(int itemTypeId);
		static void Clear();
	};
}
