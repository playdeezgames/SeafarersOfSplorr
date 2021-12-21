#pragma once
#include <set>
namespace data::game::island
{
	struct ItemLegacy
	{
		static void Write(int, int);
		static void Clear(int, int);
		static bool Read(int, int);
		static std::set<int> GetAll(int);
		static void ClearAll();
	};
}
