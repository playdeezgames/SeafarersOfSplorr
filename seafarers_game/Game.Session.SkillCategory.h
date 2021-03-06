#pragma once
#include "Game.Characteristic.h"
#include "Game.SkillCategory.h"
#include <map>
#include <string>
#include <vector>
namespace game::session
{
	struct SkillCategory
	{
		constexpr explicit SkillCategory(const game::SkillCategory& category) : category(category) {}
		std::string GetName() const;
		int GetDefaultValue() const;
		std::vector<game::Characteristic> GetCharacteristics() const;
		int GetCharacteristicCoefficient(const game::Characteristic& characteristic) const;
		constexpr explicit operator game::SkillCategory() const { return category; }
		constexpr friend bool operator<(const SkillCategory& lhs, const SkillCategory& rhs) { return lhs.category < rhs.category; }
	private:
		game::SkillCategory category;
	};
}
