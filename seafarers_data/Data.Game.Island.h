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
		int id;
		common::XY<double> location;
		std::string name;
		int patronDemigodId;

		static void Initialize();
		static std::optional<Island> Read(int);
		static std::optional<int> Find(const common::XY<double>&);
		static std::list<Island> Filter(std::function<bool(const Island&)>);

		static int Create(const common::XY<double>&, const std::string&, int);
		static std::optional<common::XY<double>> ReadLocation(int);
		static std::optional<std::string> ReadName(int);
		static std::optional<int> ReadPatronDemigodId(int);
		static std::list<int> All();
		static void Clear();
	};

}
