#pragma once
#include <list>
#include <optional>
namespace data::game::item::type
{
	struct Commodity
	{
		static void Initialize();
		static void Write(int, int, double);
		static std::optional<double> Read(int, int);
		static std::list<int> All(int);
		static void Clear();
	};
}
