#pragma once
#include <optional>
namespace data::game::avatar
{
	struct Ship
	{
		int shipId;
		int berthType;

		static void Write(int, const Ship&);
		static std::optional<Ship> Read(int);
	};
}