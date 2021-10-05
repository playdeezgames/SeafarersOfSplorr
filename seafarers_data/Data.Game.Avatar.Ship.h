#pragma once
#include <optional>
namespace data::game::avatar
{
	struct Ship
	{
		static void Write(int, int);
		static std::optional<int> Read(int);
	};
}