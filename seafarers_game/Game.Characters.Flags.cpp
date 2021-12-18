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


	const std::string& Flags::GetName(const Flag& flag)
	{
		return descriptors.find(flag)->second.name;
	}

	std::set<Flag> Flags::All(int characterId)
	{
		auto flagIds = data::game::character::Flags::All(characterId);

		return common::utility::Set::Map<int, Flag>(flagIds, [](int flagId) { return (Flag)flagId; });
	}
}