#pragma once
#include <optional>
namespace data::game::avatar
{
	struct Rations
	{
		static void Write(int, std::optional<int>);
		static std::optional<int> Read(int);
		static void Clear(int);
		static void ClearAll();
	};
}