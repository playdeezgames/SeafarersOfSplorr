#include <Common.Utility.Set.h>
#include <Data.Game.Character.Flags.h>
#include "Game.Character.Flags.h"
#include <map>
namespace game::character
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

	bool Flags::Has(int characterId, const Flag& flag)
	{
		return data::game::character::Flags::Has(characterId, (int)flag);
	}

	void Flags::Write(int characterId, const Flag& flag)
	{
		data::game::character::Flags::Write(characterId, (int)flag);
	}

	void Flags::Clear(int characterId, const Flag& flag)
	{
		data::game::character::Flags::Clear(characterId, (int)flag);
	}

	std::set<Flag> Flags::All(int characterId)
	{
		auto flagIds = data::game::character::Flags::All(characterId);

		return common::utility::Set::Map<int, Flag>(flagIds, [](int flagId) { return (Flag)flagId; });
	}
}