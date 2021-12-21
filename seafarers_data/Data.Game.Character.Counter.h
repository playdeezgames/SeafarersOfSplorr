#pragma once
#include <optional>
namespace data::game::character
{
	struct Counter
	{
		static void Initialize();
		static void Write(int, int, int);
		static std::optional<int> Read(int, int);
		static void Clear(int, int);
	};
}
