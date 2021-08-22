#pragma once
#include <Common.XY.h>
#include <list>
#include <optional>
namespace data::game::island::Known
{
	void Write(const common::XY<double>&);
	bool Read(const common::XY<double>&);
	void Clear();
	std::list<common::XY<double>> All();
}