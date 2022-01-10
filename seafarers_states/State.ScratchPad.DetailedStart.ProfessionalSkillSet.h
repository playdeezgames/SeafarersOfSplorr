#pragma once
#include <Game.Profession.h>
#include <Game.Session.Skill.h>
#include <Game.Session.SkillCategory.h>
#include <set>
namespace state::scratch_pad::detailed_start
{
	struct ProfessionalSkillSet
	{
		ProfessionalSkillSet() = delete;
		static const std::set<game::session::Skill>& GetSkills();
		static void GenerateSkills(const game::Profession& profession, const std::set<game::session::SkillCategory>& categorySet);
	private:
		static std::set<game::session::Skill> professionalSkills;
		static void AddSkillCategoryToProfessionalSkillSet(const game::session::SkillCategory& category);
		static void AddSkillCategorySetToProfessionalSkillSet(const std::set<game::session::SkillCategory>& categorySet);
	};
}
