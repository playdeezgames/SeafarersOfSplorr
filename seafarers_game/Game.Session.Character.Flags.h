#pragma once
#include "Game.Session.Character.Flag.h"
#include <list>
namespace game::session::character
{
	struct Flags
	{
		Flags(int);
		Flag GetFlag(const game::characters::Flag&) const;
		std::list<Flag> GetAll() const;
	private:
		int characterId;
	};
}