#include <Common.Utility.Set.h>
#include <Data.Game.Character.Flags.h>
#include "Game.Characters.Flags.h"
#include <map>
namespace game::characters
{
	struct FlagDescriptor
	{
		std::string name;
	};

	static const std::map<Flag, FlagDescriptor> descriptors =
	{
		{ Flag::FED, { "fed" }},
		{ Flag::UNFED, { "unfed" }}
	};
}