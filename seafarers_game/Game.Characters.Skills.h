#pragma once
#include "Game.SkillCategory.h"
#include <map>
#include <optional>
namespace game::characters
{
	struct Skills
	{
		static std::optional<int> Read(int, const SkillCategory&);
		static std::map<SkillCategory, int> Read(int);
		static void Write(int, const SkillCategory&, int);
		static void Generate(int);
	};
}
