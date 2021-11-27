#pragma once
#include <Common.XY.h>
#include <set>
namespace data::game::island
{
	struct Item
	{
		static void Write(int, int);
		static void Clear(int, int);
		static bool Read(int, int);
		static std::set<int> GetAll(int);
		static void ClearAll();
	};
}
