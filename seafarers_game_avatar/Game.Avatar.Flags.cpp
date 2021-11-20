#include <Common.Utility.Set.h>
#include <Data.Game.Avatar.Flags.h>
#include "Game.Avatar.Flags.h"
namespace game::avatar
{
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