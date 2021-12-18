#include <Data.Game.Character.Flags.h>
#include "Game.Session.Character.Flags.h"
#include <map>
#include <string>
namespace game::session::character
{
	Flags::Flags(int characterId)
		: characterId(characterId)
	{

	}

	Flag Flags::GetFlag(const game::characters::Flag& flag) const
	{
		return Flag(characterId, flag);
	}

	std::list<Flag> Flags::GetAll() const
	{
		std::list<Flag> result;
		for (auto flag : data::game::character::Flags::All(characterId))
		{
			result.push_back(Flag(characterId, (game::characters::Flag)flag));
		}
		return result;
	}
}