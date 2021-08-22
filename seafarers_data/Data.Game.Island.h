#pragma once
#include <Common.XY.h>
#include <string>
#include <optional>
#include <list>
namespace data::game::Island
{
	struct Data
	{
		common::XY<double> location;
		std::string name;
		double careeningDistance;
	};

	void Write(const Data&);
	std::optional<Data> Read(const common::XY<double>&);
	std::list<Data> All();
	void Clear();
}
