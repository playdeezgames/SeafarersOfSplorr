#pragma once
#include "Game.Session.Character.Skill.h"
namespace game::session::character
{
	struct Skills
	{
		constexpr explicit Skills(int characterId) : characterId(characterId) {}
		constexpr Skill GetSkill(int skillId) const { return Skill(characterId, skillId); }
	private:
		int characterId;
	};
}
