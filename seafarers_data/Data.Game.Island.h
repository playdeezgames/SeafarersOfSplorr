#pragma once
#include <Common.XY.h>
#include <functional>
#include <list>
#include <optional>
#include <string>
namespace data::game
{
	struct Island
	{
		common::XY<double> location;
		std::string name;
		int patronDemigodId;

		static void Write(const Island&);
		static std::optional<Island> Read(const common::XY<double>&);
		static std::list<Island> Filter(std::function<bool(const Island&)>);
		static std::list<Island> All();
		static void Clear();
	};

}
