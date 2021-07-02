#include "Game.h"
#include <map>
#include "Data.SQLite.Stores.h"
namespace game
{
	void Reset(const Difficulty& difficulty)
	{
		//TODO: figure this out!
	}

	void Start()
	{
		Reset(Difficulty::NORMAL);
	}

	void AutoSave()
	{
		data::sqlite::Stores::Copy(data::sqlite::Store::IN_MEMORY, data::sqlite::Store::AUTOSAVE);
	}

	void LoadFromAutosave()
	{
		data::sqlite::Stores::Copy(data::sqlite::Store::AUTOSAVE, data::sqlite::Store::IN_MEMORY);
	}

	bool DoesAutosaveExist()
	{
		return false;//TODO: figure this out!
	}

	bool DoesSlotExist(int slot)
	{
		return false;//TODO: figure this out!
	}

	const std::map<int, data::sqlite::Store> slotTable =
		{
			{0, data::sqlite::Store::SLOT_1},
			{1, data::sqlite::Store::SLOT_2},
			{2, data::sqlite::Store::SLOT_3},
			{3, data::sqlite::Store::SLOT_4},
			{4, data::sqlite::Store::SLOT_5}
		};

	void LoadFromSlot(int slot)
	{
		auto iter = slotTable.find(slot);
		if (iter != slotTable.end())
		{
			data::sqlite::Stores::Copy(iter->second, data::sqlite::Store::IN_MEMORY);
		}
	}

	void SaveToSlot(int slot)
	{
		auto iter = slotTable.find(slot);
		if (iter != slotTable.end())
		{
			data::sqlite::Stores::Copy(data::sqlite::Store::IN_MEMORY, iter->second);
		}
	}

}