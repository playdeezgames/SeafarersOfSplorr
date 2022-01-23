#pragma once
#include "Game.SkillCategory.h"
#include "Game.Profession.h"
#include <set>
#include <string>
namespace game::session::world
{
	struct Profession
	{
		constexpr explicit Profession(const game::Profession profession) : profession(profession) {}
		const std::string& GetName() const;
		constexpr explicit operator game::Profession() const { return profession; }
		const std::set<game::SkillCategory>& GetSkillCategories() const;
		const std::set<game::SkillCategory>& GetElectiveSkillCategories() const;
	private:
		game::Profession profession;
	};
}
