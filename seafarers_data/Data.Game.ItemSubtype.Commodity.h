#pragma once
#include <list>
#include <optional>
namespace data::game::item_subtype
{
	struct Commodity
	{
		static void Write(int, int, double);
		static std::optional<double> Read(int, int);
		static std::list<int> All(int);
		static void Clear();
	};
}
