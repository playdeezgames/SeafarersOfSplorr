#pragma once
#include "Common.XY.h"
#include <optional>
#include <map>
namespace data::game::island::Market
{
	struct MarketData
	{
		double supply;
		double demand;
		double purchases;
		double sales;
	};

	void Write(const common::XY<double>&, int, const MarketData&);
	std::optional<MarketData> Read(const common::XY<double>&, int);
	std::map<int, MarketData> All(const common::XY<double>&);
	void Clear();
}