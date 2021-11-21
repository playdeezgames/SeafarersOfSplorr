#include <Common.Utility.Set.h>
#include <Data.Game.Avatar.Flags.h>
#include "Game.Avatar.Flags.h"
#include <map>
namespace game::avatar
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

	bool Flags::Has(int avatarId, const Flag& flag)
	{
		return data::game::avatar::Flags::Has(avatarId, (int)flag);
	}

	void Flags::Write(int avatarId, const Flag& flag)
	{
		data::game::avatar::Flags::Write(avatarId, (int)flag);
	}

	void Flags::Clear(int avatarId, const Flag& flag)
	{
		data::game::avatar::Flags::Clear(avatarId, (int)flag);
	}

	std::set<Flag> Flags::All(int avatarId)
	{
		auto flagIds = data::game::avatar::Flags::All(avatarId);

		return common::utility::Set::Map<int, Flag>(flagIds, [](int flagId) { return (Flag)flagId; });
	}
}