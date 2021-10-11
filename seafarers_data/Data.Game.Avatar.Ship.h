#pragma once
#include <optional>
namespace data::game::avatar//20211011
{
	struct Ship
	{
		int shipId;
		int berthType;

		static void Write(int, const Ship&);
		static std::optional<Ship> Read(int);
	};
}