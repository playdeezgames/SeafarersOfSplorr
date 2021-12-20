#pragma once
#include <optional>
#include <list>
namespace data::game::character
{
	struct PlightLegacy
	{
		int plightId;
		std::optional<int> duration;

		static std::optional<PlightLegacy> Read(int, int);
		static void Write(int, const PlightLegacy&);
		static void ClearPlight(int, int);
		static void Clear(int);
		static std::list<PlightLegacy> All(int);

		static void ClearAll();
	};
}
