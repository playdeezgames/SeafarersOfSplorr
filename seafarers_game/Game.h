#pragma once
#include "Game.Difficulty.h"
#include <functional>
namespace game
{
	std::function<void()> DoAddResetter(std::function<void()>);//TODO: legacize
	void ResetLegacy(const Difficulty&);
	void AutoSave();
	bool DoesAutosaveExist();
	void LoadFromAutosave();
	bool DoesSlotExist(int);
	void LoadFromSlot(int);
	void SaveToSlot(int);
}
