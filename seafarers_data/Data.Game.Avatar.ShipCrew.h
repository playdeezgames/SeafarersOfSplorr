#pragma once
#include <vector>
namespace data::game::avatar//20211011
{
	struct ShipCrew
	{
		int avatarId;
		int berthType;
		static std::vector<ShipCrew> Read(int);
	};
}