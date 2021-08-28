#pragma once
#include "Game.Demigod.h"
#include "Game.Difficulty.h"
#include "Game.Item.h"
#include <list>
namespace game
{
	struct Demigods
	{
		static void Reset(const Difficulty&);
		static void MakeOffering(const std::string&, const Item&);
	};
}