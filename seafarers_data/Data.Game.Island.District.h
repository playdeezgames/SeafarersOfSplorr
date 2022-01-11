#pragma once
#include <vector>
namespace data::game::island
{
	struct District
	{
		District() = delete;
		static void Initialize();
		static std::vector<int> Read(int islandId);
	};
}
