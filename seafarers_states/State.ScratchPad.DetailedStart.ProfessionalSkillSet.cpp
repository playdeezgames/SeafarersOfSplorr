#include "State.ScratchPad.DetailedStart.ProfessionalSkillSet.h"
#include <algorithm>
#include <Game.Session.World.Professions.h>
#include <Game.Session.Skills.h>
#include <iterator>
namespace state::scratch_pad::detailed_start
{
	std::set<game::session::Skill> ProfessionalSkillSet::professionalSkills;

	const std::set<game::session::Skill>& ProfessionalSkillSet::GetSkills()
	{
		return professionalSkills;
	}

	void ProfessionalSkillSet::AddSkillCategoryToProfessionalSkillSet(const game::SkillCategory& category)
	{
		auto skills =
			game::session::Skills::GetSkillsInCategory(category);
		std::copy(
			skills.begin(),
			skills.end(),
			std::inserter(professionalSkills, professionalSkills.end()));
	}

	void ProfessionalSkillSet::AddSkillCategorySetToProfessionalSkillSet(const std::set<game::SkillCategory>& categorySet)
	{
		std::for_each(
			categorySet.begin(),
			categorySet.end(),
			AddSkillCategoryToProfessionalSkillSet);
	}

	void ProfessionalSkillSet::GenerateSkills(const game::Profession& profession, const std::set<game::SkillCategory>& categorySet)
	{
		professionalSkills.clear();

		AddSkillCategorySetToProfessionalSkillSet(
			game::session::world::Professions::GetProfession(profession)
			.GetSkillCategories());

		AddSkillCategorySetToProfessionalSkillSet(
			categorySet);
	}
}