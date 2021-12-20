#pragma once
#include <optional>
namespace data::game::character
{
	struct RationsLegacy
	{
		static void Write(int, int);
		static std::optional<int> Read(int);
		static void Clear(int);
		static void ClearAll();
	};
}