#pragma once
#include "Game.Characters.Flag.h"
#include <set>
#include <string>
namespace game::characters
{
	struct Flags
	{
		static std::set<Flag> All(int);
		static const std::string& GetName(const Flag&);
	};
}
