#pragma once
namespace game
{
	void AutoSave();
	bool DoesAutosaveExist();
	void LoadFromAutosave();

	bool DoesSlotExist(int);
	void LoadFromSlot(int);
	void SaveToSlot(int);
}
