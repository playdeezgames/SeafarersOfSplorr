#include <Data.Game.Character.Flags.h>
#include "Game.Session.Character.Flag.h"
namespace game::session::character
{
	Flag::Flag(int characterId, const game::characters::Flag& flag)
		: characterId(characterId)
		, flag(flag)
	{

	}

	bool Flag::Has() const
	{
		return data::game::character::Flags::Has(characterId, (int)flag);
	}

	void Flag::Set() const
	{
		data::game::character::Flags::Write(characterId, (int)flag);
	}

	void Flag::Reset() const
	{
		data::game::character::Flags::Clear(characterId, (int)flag);
	}
}


