#pragma once
#include "Game.Session.Character.h"
namespace game::session
{
	struct Player
	{
		Character GetCharacter() const;
	};
}
