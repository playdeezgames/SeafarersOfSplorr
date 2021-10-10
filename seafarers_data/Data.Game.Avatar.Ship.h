#pragma once
#include <optional>
#include <vector>
namespace data::game::avatar
{
	struct ShipCrew
	{
		int avatarId;
		int berthType;
		static std::vector<ShipCrew> Read(int);
	};
	struct Ship
	{
		int shipId;
		int berthType;

		static void Write(int, const Ship&);
		static std::optional<Ship> Read(int);
	};
}