#include <Data.Game.Character.Flags.h>
#include "Game.Session.Character.Flag.h"
#include <map>
namespace game::session::character
{
	using FlagData = data::game::character::Flags;

	struct FlagDescriptor
	{
		std::string name;
	};

	static const std::map<game::characters::Flag, FlagDescriptor> descriptors =
	{
		{ game::characters::Flag::FED, { "fed" }},
		{ game::characters::Flag::UNFED, { "unfed" }}
	};

	bool Flag::Has() const
	{
		return FlagData::Has(characterId, (int)flag);
	}

	void Flag::Set() const
	{
		FlagData::Write(characterId, (int)flag);
	}

	void Flag::Reset() const
	{
		FlagData::Clear(characterId, (int)flag);
	}

	const std::string& Flag::GetName() const
	{
		return descriptors.find(flag)->second.name;
	}

}


