#pragma once
#include <vector>
namespace data::game::character
{
	struct ShipCrew
	{
		int characterId;
		int berthType;
		static std::vector<ShipCrew> Read(int);
	};
}