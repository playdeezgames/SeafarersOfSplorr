#pragma once
#include <optional>
namespace data::game::island
{
	struct VisitLegacy
	{
		int islandId;
		int visits;
		std::optional<int> lastVisit;

		static void Write(const VisitLegacy&);
		static std::optional<VisitLegacy> Read(int);
		static void Clear();
	};

}