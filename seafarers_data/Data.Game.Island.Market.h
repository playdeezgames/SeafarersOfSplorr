#pragma once
#include <map>
#include <optional>
namespace data::game::island
{
	struct Market
	{
		Market() = delete;
		struct Data
		{
			double supply;
			double demand;
			double purchases;
			double sales;
		};
		static void Initialize();
		static void Write(int islandId, int commodity, const Data& marketData);
		static std::optional<Data> Read(int islandId, int commodity);
		static std::map<int, Data> All(int islandId);
		static void Clear();
	};

}