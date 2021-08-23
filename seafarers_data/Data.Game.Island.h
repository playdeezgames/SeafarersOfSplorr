#pragma once
#include <Common.XY.h>
#include <list>
#include <optional>
#include <string>
namespace data::game
{
	struct Island
	{
		common::XY<double> location;
		std::string name;
		double careeningDistance;
		static void Write(const Island&);
		static std::optional<Island> Read(const common::XY<double>&);
		static std::list<Island> All();
		static void Clear();
	};

}
