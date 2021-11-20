#pragma once
#include "Game.Avatar.Flag.h"
#include <set>
namespace game::avatar
{
	struct Flags
	{
		static bool Has(int, const Flag&);
		static void Write(int, const Flag&);
		static void Clear(int, const Flag&);
		static std::set<Flag> All(int);
	};
}
