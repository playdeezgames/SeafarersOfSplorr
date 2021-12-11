#pragma once
#include "Game.Skill.h"
#include <map>
#include <optional>
namespace game::characters
{
	struct Skills
	{
		static std::optional<int> Read(int, const Skill&);
		static std::map<Skill, int> Read(int);
		static void Write(int, const Skill&, int);
		static void Generate(int);
	};
}
