#include <Data.SQLite.Store.h>
#include "Game.Session.Saves.h"
namespace game::session
{
	Save Saves::GetAuto() const
	{
		return Save((int)data::sqlite::Store::AUTOSAVE);
	}

	Save Saves::GetSlot1() const
	{
		return Save((int)data::sqlite::Store::SLOT_1);
	}

	Save Saves::GetSlot2() const
	{
		return Save((int)data::sqlite::Store::SLOT_2);
	}

	Save Saves::GetSlot3() const
	{
		return Save((int)data::sqlite::Store::SLOT_3);
	}

	Save Saves::GetSlot4() const
	{
		return Save((int)data::sqlite::Store::SLOT_4);
	}

	Save Saves::GetSlot5() const
	{
		return Save((int)data::sqlite::Store::SLOT_5);
	}
}