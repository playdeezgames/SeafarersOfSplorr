#pragma once
#include <set>
namespace data::game::character
{
	struct Flags
	{
		static bool Has(int, int);
		static void Write(int, int);
		static void Clear(int, int);
		static std::set<int> All(int);
	};
}