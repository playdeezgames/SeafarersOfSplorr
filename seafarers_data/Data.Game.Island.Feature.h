#pragma once
#include "Common.XY.h"
namespace data::game::island::Feature
{
	void ClearAll();
	void Write(const common::XY<double>&, int);
	void Clear(const common::XY<double>&, int);
	bool Read(const common::XY<double>&, int);
}
