#pragma once
#include <Game.BerthType.h>
#include <string>
#include <vector>
namespace game::ship//20211017
{
	struct Crew
	{
		int avatarId;
		BerthType berthType;
		std::string name;

		static std::vector<Crew> Read();
	};
}
