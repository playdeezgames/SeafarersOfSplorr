#pragma once
#include <optional>
#include <vector>
namespace data::game::character
{
	struct Ship
	{
		int shipId;
		int characterId;
		int berthType;

		static void Write(int, const Ship&);
		static std::optional<Ship> ReadForCharacter(int);
		static std::vector<Ship> ReadForShip(int);
	};
}