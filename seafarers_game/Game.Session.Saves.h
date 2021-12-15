#pragma once
#include "Game.Session.Save.h"
namespace game::session
{
	struct Saves
	{
		Save GetAuto() const;
		Save GetSlot1() const;
		Save GetSlot2() const;
		Save GetSlot3() const;
		Save GetSlot4() const;
		Save GetSlot5() const;
	};
}