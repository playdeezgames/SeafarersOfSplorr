#pragma once
#include <Game.SkillCategory.h>
#include <Game.Profession.h>
#include <Game.Session.Skill.h>
#include <set>
namespace state::scratch_pad::detailed_start
{
	struct ProfessionalSkillSet
	{
		ProfessionalSkillSet() = delete;
		static const std::set<game::session::Skill>& GetSkills();
		static void GenerateSkills(const game::Profession& profession, const std::set<game::SkillCategory>& categorySet);
	private:
		static std::set<game::session::Skill> professionalSkills;
		static void AddSkillCategoryToProfessionalSkillSet(const game::SkillCategory& category);
		static void AddSkillCategorySetToProfessionalSkillSet(const std::set<game::SkillCategory>& categorySet);
	};
}
