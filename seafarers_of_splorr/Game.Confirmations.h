#pragma once
#include "Game.Confirmation.h"
#include "Game.Difficulty.h"
namespace game::Confirmations
{
	bool HasConfirmation();
	Confirmation Read();
	void Write(const Confirmation&);
	void Reset(const Difficulty&);
}
