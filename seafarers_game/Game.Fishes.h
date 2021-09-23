#pragma once
#include "Game.FishDescriptor.h"
#include <list>
namespace game
{
	struct Fishes
	{
		static std::list<Fish> All();
		static std::list<Fish> AllJunk();
		static const FishDescriptor& Read(const Fish&);
	};
}
