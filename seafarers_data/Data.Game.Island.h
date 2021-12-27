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
		static void Initialize();
		static int Create(const common::XY<double>&, const std::string&, int);
		static std::optional<common::XY<double>> ReadLocation(int);
		static std::optional<std::string> ReadName(int);
		static std::optional<int> ReadPatronDemigodId(int);
		static std::list<int> All();
		static void Clear();
	};

}
