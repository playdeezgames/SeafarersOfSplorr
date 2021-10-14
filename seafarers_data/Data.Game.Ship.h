#pragma once
#include <Common.XY.h>
#include <optional>
#include <string>
#include <vector>
namespace data::game//20211014
{
	struct Ship
	{
		int shipId;
		int shipType;
		std::string name;
		common::XY<double> location;
		double heading;
		double speed;

		static void Write(const Ship&);
		static std::optional<Ship> Read(int);
		static void Clear();

		static int NextId();
	};
}
