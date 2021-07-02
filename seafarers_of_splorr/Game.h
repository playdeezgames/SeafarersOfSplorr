#pragma once
#include "Game.Difficulty.h"
namespace game
{
	void Start();
	void Reset(const Difficulty&);
	void AutoSave();
	bool DoesAutosaveExist();
	void LoadFromAutosave();
	bool DoesSlotExist(int);
	void LoadFromSlot(int);
	void SaveToSlot(int);
}
