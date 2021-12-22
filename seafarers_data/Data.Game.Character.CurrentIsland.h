#pragma once
#include <list>
#include <optional>
namespace data::game::character
{
	struct Island
	{
		static std::optional<int> Read(int);
		static void Write(int, int);
		static void Clear(int);
		static std::list<int> All(int);//TODO: make into AllForIsland(int)
	};
}
