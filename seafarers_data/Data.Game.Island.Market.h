#pragma once
#include <Common.XY.h>
#include <map>
#include <optional>
namespace data::game::island
{
	struct Market
	{
		double supply;
		double demand;
		double purchases;
		double sales;

		static void Write(int, int, const Market&);
		static std::optional<Market> Read(int, int);
		static std::map<int, Market> All(int);
		static void Clear();
	};

}