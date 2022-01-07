#pragma once
#include "Game.Session.SkillCategory.h"
#include "Game.SkillCategory.h"
#include "Game.Profession.h"
#include <string>
#include <vector>
namespace game::session::world
{
	struct Profession
	{
		constexpr explicit Profession(const game::Profession profession) : profession(profession) {}
		std::string GetName() const;
		constexpr explicit operator game::Profession() const { return profession; }
		const std::vector<SkillCategory>& GetSkillCategories() const;
		const std::vector<SkillCategory>& GetElectiveSkillCategories() const;
	private:
		game::Profession profession;
	};
}
