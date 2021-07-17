#pragma once
#include "Common.XY.h"
namespace data::game::island::Item
{
	void Set(const common::XY<double>&, int);
	void Clear(const common::XY<double>&, int);
	bool Get(const common::XY<double>&, int);
	void ClearAll();
}
