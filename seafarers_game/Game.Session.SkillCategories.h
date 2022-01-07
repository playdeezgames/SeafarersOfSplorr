#pragma once
#include "Game.SkillCategory.h"
#include "Game.Session.SkillCategory.h"
#include <vector>
namespace game::session
{
	struct SkillCategories
	{
		constexpr SkillCategories() {}
		const std::vector<SkillCategory>& GetSkillCategories() const;
	};
}
