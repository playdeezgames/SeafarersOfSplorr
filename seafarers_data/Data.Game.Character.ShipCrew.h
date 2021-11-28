#pragma once
#include <vector>
namespace data::game::character
{
	struct ShipCrew
	{
		int characterId;
		int berthType;
		//TODO: add shipId
		//TODO: merge with Ship
		static std::vector<ShipCrew> ReadForShip(int);
	};
}