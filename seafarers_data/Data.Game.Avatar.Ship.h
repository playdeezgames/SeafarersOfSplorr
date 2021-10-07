#pragma once
#include <optional>
namespace data::game::avatar
{
	struct Ship
	{
		int shipId;

		static void Write(int, int);
		static std::optional<Ship> Read(int);
	};
}