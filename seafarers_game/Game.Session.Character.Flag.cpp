#include <Data.Game.Character.Flags.h>
#include "Game.Session.Character.Flag.h"
#include <map>
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

	const std::string& Flag::GetName() const
	{
		return descriptors.find(flag)->second.name;
	}

}


