#pragma once
#include <string>
namespace game::session
{
	struct Skill
	{
		constexpr Skill(int skillId) : skillId(skillId) {}
		std::string GetName() const;
		constexpr friend bool operator<(const Skill& lhs, const Skill& rhs) { return lhs.skillId < rhs.skillId; }
		constexpr static Skill ToSkill(int skillId) { return Skill(skillId); }
	private:
		int skillId;
	};
}
