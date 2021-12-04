#pragma once
#include "Game.Characters.Flag.h"
#include <set>
#include <string>
namespace game::characters
{
	struct Flags
	{
		static bool Has(int, const Flag&);
		static void Write(int, const Flag&);
		static void Clear(int, const Flag&);
		static std::set<Flag> All(int);
		static const std::string& GetName(const Flag&);
	};
}
