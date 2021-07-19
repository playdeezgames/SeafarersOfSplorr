#pragma once
#include "Common.XY.h"
#include <set>
namespace data::game::island::Item
{
	void Set(const common::XY<double>&, int);
	void Clear(const common::XY<double>&, int);
	bool Get(const common::XY<double>&, int);
	std::set<int> GetAll(const common::XY<double>&);
	void ClearAll();
}
