#pragma once
#include <list>
#include <optional>
namespace data::game::item::type
{
	struct Commodity
	{
		Commodity() = delete;
		static void Initialize();
		static void Write(int itemTypeId, int commodity, double amount);
		static std::optional<double> ReadAmount(int itemTypeId, int commodity);
		static std::list<int> AllCommodities(int itemTypeId);
		static void Clear();
	};
}
