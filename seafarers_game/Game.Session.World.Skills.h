#pragma once
#include "Game.Difficulty.h"
#include "Game.Session.Skill.h"
#include <set>
namespace game::session::world
{
	struct Skills
	{
		constexpr Skills() {}
		void Reset() const;
		void Populate(const Difficulty& difficulty) const;
		constexpr Skill GetSkill(int skillId) const { return Skill(skillId); }
		std::set<Skill> GetSkillsInCategory(const game::SkillCategory& category) const;
		std::set<Skill> GetSkills() const;
	};
}
