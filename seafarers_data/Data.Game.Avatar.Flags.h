#pragma once
#include <set>
namespace data::game::avatar
{
	struct Flags
	{
		static bool Has(int, int);
		static void Write(int, int);
		static void Clear(int, int);
		static std::set<int> All(int);
		//ClearAll(int)
		//ClearAll()
	};
}