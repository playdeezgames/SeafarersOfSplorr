#pragma once
#include <functional>
#include "Game.Fish.h"
#include <string>
namespace game
{
	struct FishDescriptor
	{
		std::string name;
		std::function<size_t()> countGenerator;
		std::function<double()> radiusGenerator;
		std::function<double()> speedGenerator;
	};
}
