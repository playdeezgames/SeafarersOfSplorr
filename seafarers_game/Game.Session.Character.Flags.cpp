#include <algorithm>
#include <Data.Game.Character.Flags.h>
#include "Game.Session.Character.Flags.h"
#include <map>
#include <string>
#include <iterator>
namespace game::session::character
{
	std::list<Flag> Flags::GetAll() const
	{
		std::list<Flag> result;
		auto flags = data::game::character::Flags::All(characterId);
		std::transform(
			flags.begin(),
			flags.end(),
			std::back_inserter(result),
			[this](int flag) 
			{
				return Flag(characterId, (game::characters::Flag)flag);
			});
		return result;
	}
}