#pragma once
#include <optional>
#include <list>
namespace data::game::character
{
	struct PlightLegacy
	{
		static void Initialize();
		static std::list<int> All(int);
		static void Write(int, int, int);
		static std::optional<int> ReadDuration(int, int);
		static void ClearPlight(int, int);
		static void Clear(int);

		static void ClearAll();
	};
}
