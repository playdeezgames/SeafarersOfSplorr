#pragma once
#include <optional>
#include "Common.XY.h"
#include <list>
namespace data::game::island::Known
{
	void Write(const common::XY<double>&);
	bool Read(const common::XY<double>&);
	void Clear();
	std::list<common::XY<double>> All();
}