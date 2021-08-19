#pragma once
#include <functional>
#include "Game.Difficulty.h"
namespace game
{
	void AddResetter(std::function<void()>);
	std::function<void()> DoAddResetter(std::function<void()>);
	void Start();
	void Reset(const Difficulty&);
	void AutoSave();
	bool DoesAutosaveExist();
	void LoadFromAutosave();
	bool DoesSlotExist(int);
	void LoadFromSlot(int);
	void SaveToSlot(int);
}
