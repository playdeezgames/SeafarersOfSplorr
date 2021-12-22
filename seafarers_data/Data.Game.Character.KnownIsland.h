#pragma once
#include <list>
namespace data::game::character
{
	struct KnownIsland
	{
		static void Initialize();
		static void Write(int, int);
		static bool Read(int, int);
		static void Clear(int);
		static void Clear();
		static std::list<int> All(int);
	};
}
