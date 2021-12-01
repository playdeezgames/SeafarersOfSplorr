#pragma once
#include <optional>
namespace data::game::character
{
	struct Island
	{
		static std::optional<int> Read(int);
		static void Write(int, int);
		static void Clear(int);
	};
}
