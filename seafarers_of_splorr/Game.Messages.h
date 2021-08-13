#pragma once
#include "Game.Difficulty.h"
#include "Game.Message.h"
namespace game::Messages
{
	bool HasMessage();
	Message Read();
	void Write(const Message&);
	void Reset(const Difficulty&);
}
