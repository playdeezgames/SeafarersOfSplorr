#pragma once
#include <string>
#include <vector>
namespace game
{
	struct ShipNames
	{
		static const std::vector<std::string>& GetAdjectives();
		static const std::vector<std::string>& GetNouns();
		static std::string Generate();
	};
}