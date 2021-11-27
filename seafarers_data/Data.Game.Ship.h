#pragma once
#include <Common.XY.h>
#include <optional>
#include <string>
#include <vector>
#include <list>
namespace data::game
{
	struct Ship
	{
		int shipId;
		int shipType;
		std::string name;
		common::XY<double> location;
		double heading;
		double speed;

		static int Write(const Ship&);
		static std::optional<Ship> Read(int);
		static void Clear();

		static std::list<int> All();
	};
}
