#pragma once
#include <optional>
namespace data::game::island
{
	struct Visit
	{
		int islandId;
		int visits;
		std::optional<int> lastVisit;

		static void Write(const Visit&);
		static std::optional<Visit> Read(int);
		static void Clear();
	};

}