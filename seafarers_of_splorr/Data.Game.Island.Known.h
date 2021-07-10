#pragma once
#include <optional>
#include "Common.XY.h"
namespace data::game::island::Known
{
	void Write(const common::XY<double>&);
	bool Read(const common::XY<double>&);
	void Clear();
}