#include <Data.Game.Skill.h>
#include "Game.Session.Skill.h"
namespace game::session
{
	std::string Skill::GetName() const
	{
		return data::game::Skill::ReadName(skillId).value();
	}
}