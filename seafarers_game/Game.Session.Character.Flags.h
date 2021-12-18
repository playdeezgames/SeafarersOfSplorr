#pragma once
#include "Game.Session.Character.Flag.h"
namespace game::session::character
{
	struct Flags
	{
		Flags(int);
		Flag GetFlag(const game::characters::Flag&) const;
	private:
		int characterId;
	};
}