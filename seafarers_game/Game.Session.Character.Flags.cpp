#include <Data.Game.Character.Flags.h>
#include "Game.Session.Character.Flags.h"
#include <map>
#include <string>
namespace game::session::character
{
	struct FlagDescriptor
	{
		std::string name;
	};

	static const std::map<game::characters::Flag, FlagDescriptor> descriptors =
	{
		{ game::characters::Flag::FED, { "fed" }},
		{ game::characters::Flag::UNFED, { "unfed" }}
	};

	Flags::Flags(int characterId)
		: characterId(characterId)
	{

	}

	Flag Flags::GetFlag(const game::characters::Flag& flag) const
	{
		return Flag(characterId, flag);
	}
}