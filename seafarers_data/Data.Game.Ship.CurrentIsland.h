#pragma once
#include <optional>
namespace data::game::ship
{
	struct CurrentIsland
	{
		static void Initialize();
		static void Write(int, int);
		static std::optional<int> Read(int);
		static void Clear(int);
		static void ClearAll();
	};
}
