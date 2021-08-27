#pragma once
#include <optional>
namespace data::game::avatar
{
	struct Plight
	{
		int plightId;
		std::optional<int> duration;
		static std::optional<Plight> Read(int);
		static void Write(const Plight&);
		static void Clear(int);
		static void Clear();
	};
}
