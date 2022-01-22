#pragma once
#include "Game.Difficulty.h"
#include "Game.Session.Skill.h"
#include "Game.SkillCategory.h"
#include <set>
namespace game::session
{
	struct Skills
	{
		Skills() = delete;
		static void Reset();
		static void Populate(const Difficulty& difficulty);
		static constexpr Skill GetSkill(int skillId) { return Skill(skillId); }
		static std::set<Skill> GetSkillsInCategory(const game::SkillCategory& category);
		static std::set<Skill> GetSkills();
	};
}
