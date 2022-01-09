#include <Data.Game.Skill.h>
#include <Data.Game.Character.Skill.h>
#include "Game.Session.Character.Skill.h"
namespace game::session::character
{
	void Skill::AllocatePoints(size_t points) const
	{
		auto value = data::game::character::Skill::Read(characterId, skillId);
		if (!value)
		{
			value = data::game::Skill::ReadDefaultValue(skillId).value();
		}
		value.value() += (int)points;
		data::game::character::Skill::Write(characterId, skillId, value.value());
	}
}