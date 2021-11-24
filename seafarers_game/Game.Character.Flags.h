#pragma once
#include "Game.Character.Flag.h"
#include <set>
#include <string>
namespace game::character
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