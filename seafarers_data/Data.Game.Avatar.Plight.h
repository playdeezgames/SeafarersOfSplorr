#pragma once
#include <optional>
#include <list>
namespace data::game::avatar
{
	struct Plight
	{
		int plightId;
		std::optional<int> duration;

		static std::optional<Plight> Read(int, int);
		static void Write(int, const Plight&);
		static void ClearPlight(int, int);
		static void Clear(int);
		static std::list<Plight> All(int);

		static void ClearAll();
	};
}
