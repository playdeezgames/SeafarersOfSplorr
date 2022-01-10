#include <algorithm>
#include <Game.Session.h>
#include <iterator>
#include "State.ScratchPad.DetailedStart.ProfessionalSkillSet.h"
namespace state::scratch_pad::detailed_start
{
	std::set<game::session::Skill> ProfessionalSkillSet::professionalSkills;

	const std::set<game::session::Skill>& ProfessionalSkillSet::GetSkills()
	{
		return professionalSkills;
	}

	void ProfessionalSkillSet::AddSkillCategoryToProfessionalSkillSet(const game::session::SkillCategory& category)
	{
		auto skills =
			game::Session()
			.GetWorld()
			.GetSkills()
			.GetSkillsInCategory(category.operator game::SkillCategory());
		std::copy(
			skills.begin(),
			skills.end(),
			std::inserter(professionalSkills, professionalSkills.end()));
	}

	void ProfessionalSkillSet::AddSkillCategorySetToProfessionalSkillSet(const std::set<game::session::SkillCategory>& categorySet)
	{
		std::for_each(
			categorySet.begin(),
			categorySet.end(),
			AddSkillCategoryToProfessionalSkillSet);
	}

	void ProfessionalSkillSet::GenerateSkills(const game::Profession& profession, const std::set<game::session::SkillCategory>& categorySet)
	{
		professionalSkills.clear();

		AddSkillCategorySetToProfessionalSkillSet(
			game::Session()
			.GetWorld()
			.GetProfessions()
			.GetProfession(profession)
			.GetSkillCategories());

		AddSkillCategorySetToProfessionalSkillSet(
			categorySet);
	}
}