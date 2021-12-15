#pragma once
#include "Game.Session.Character.h"
namespace game::session
{
	struct Characters
	{
		Character GetCharacter(int) const;
	};
}
