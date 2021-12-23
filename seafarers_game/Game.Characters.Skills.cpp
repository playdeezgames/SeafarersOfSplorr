#include <Data.Game.Character.Skill.h>
#include "Game.Characters.Skills.h"
namespace game::characters
{
	std::optional<int> Skills::Read(int characterId, const SkillCategory& skill)
	{
		auto value = data::game::character::Skill::Read(characterId, (int)skill);
		if (value)
		{
			return (int)value.value();
		}
		return std::nullopt;
	}

	std::map<SkillCategory, int> Skills::Read(int characterId)
	{
		std::map<SkillCategory, int> result;
		auto skills = data::game::character::Skill::Read(characterId);
		for (auto skill : skills)
		{
			result[(SkillCategory)skill.first] = skill.second;
		}
		return result;
	}

	void Skills::Write(int characterId, const SkillCategory& skill, int value)
	{
		data::game::character::Skill::Write(characterId, (int)skill, value);
	}

	void Skills::Generate(int characterId)
	{
		//TODO: generate skills from skill descriptors.
		//TODO: make skill descriptors
	}

}