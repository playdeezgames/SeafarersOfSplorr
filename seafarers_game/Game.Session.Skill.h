#pragma once
#include <string>
namespace game::session
{
	struct Skill
	{
		constexpr Skill(int skillId) : skillId(skillId) {}
		constexpr int ToId() const { return skillId; }
		constexpr friend bool operator<(const Skill& lhs, const Skill& rhs) { return lhs.ToId() < rhs.ToId(); }
		constexpr static Skill ToSkill(int skillId) { return Skill(skillId); }
		std::string GetName() const;
	private:
		int skillId;
	};
}
