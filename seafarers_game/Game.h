#pragma once
#include "Game.Difficulty.h"
#include <functional>
namespace game//20211013
{
	std::function<void()> DoAddResetter(std::function<void()>);
	void Start();
	void Reset(const Difficulty&);
	void AutoSave();
	bool DoesAutosaveExist();
	void LoadFromAutosave();
	bool DoesSlotExist(int);
	void LoadFromSlot(int);
	void SaveToSlot(int);
	void ApplyTurnEffects();
}
