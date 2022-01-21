#include <algorithm>
#include <Data.Game.Character.Flags.h>
#include "Game.Session.Character.Flags.h"
#include <iterator>
namespace game::session::character
{
	std::vector<Flag> Flags::GetAll() const
	{
		auto flags = data::game::character::Flags::All(characterId);
		std::vector<Flag> result;
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