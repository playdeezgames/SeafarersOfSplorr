#include <Data.Game.Character.Skill.h>
#include "Game.Characters.Skills.h"
namespace game::characters
{
	std::optional<int> Skills::Read(int characterId, const Skill& skill)
	{
		auto value = data::game::character::Skill::Read(characterId, (int)skill);
		if (value)
		{
			return (int)value.value();
		}
		return std::nullopt;
	}

	std::map<Skill, int> Skills::Read(int characterId)
	{
		std::map<Skill, int> result;
		auto skills = data::game::character::Skill::Read(characterId);
		for (auto skill : skills)
		{
			result[(Skill)skill.first] = skill.second;
		}
		return result;
	}

	void Skills::Write(int characterId, const Skill& skill, int value)
	{
		data::game::character::Skill::Write(characterId, (int)skill, value);
	}

	void Skills::Generate(int characterId)
	{
		//TODO: generate skills from skill descriptors.
		//TODO: make skill descriptors
	}

}