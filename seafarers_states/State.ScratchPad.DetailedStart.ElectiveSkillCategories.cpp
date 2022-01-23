#include "State.ScratchPad.DetailedStart.ElectiveSkillCategories.h"
namespace state::scratch_pad::detailed_start
{
	std::set<game::SkillCategory> ElectiveSkillCategories::electedSkillCategories;

	void ElectiveSkillCategories::Clear()
	{
		electedSkillCategories.clear();
	}

	const std::set<game::SkillCategory>& ElectiveSkillCategories::GetCategories()
	{
		return electedSkillCategories;
	}

	void ElectiveSkillCategories::AddCategory(const game::SkillCategory& category)
	{
		electedSkillCategories.insert(category);
	}
}