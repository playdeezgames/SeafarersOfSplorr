#pragma once
#include <Game.SkillCategory.h>
#include <set>
namespace state::scratch_pad::detailed_start
{
	struct ElectiveSkillCategories
	{
		ElectiveSkillCategories() = delete;
		static const size_t SKILL_CATEGORY_COUNT = 10;
		static void Clear();
		static const std::set<game::SkillCategory>& GetCategories();
		static void AddCategory(const game::SkillCategory& category);
	private:
		static std::set<game::SkillCategory> electedSkillCategories;
	};
}
