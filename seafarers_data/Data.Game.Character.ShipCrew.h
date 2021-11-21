#pragma once
#include <vector>
namespace data::game::character
{
	struct ShipCrew
	{
		int avatarId;
		int berthType;
		static std::vector<ShipCrew> Read(int);
	};
}