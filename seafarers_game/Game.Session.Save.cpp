#include <Data.SQLite.Store.h>
#include <Data.SQLite.Stores.h>
#include "Game.Session.Save.h"
namespace game::session
{
	Save::Save(int storeIndex)
		: storeIndex(storeIndex)
	{

	}

	bool Save::Exists()
	{
		return !data::sqlite::Stores::Execute(storeIndex, "SELECT name FROM sqlite_master WHERE type='table' AND name='Worlds';").empty();
	}

	void Save::Store()
	{
		data::sqlite::Stores::Copy(data::sqlite::Store::IN_MEMORY, (data::sqlite::Store)storeIndex);
	}

	void Save::Load()
	{
		data::sqlite::Stores::Copy((data::sqlite::Store)storeIndex, data::sqlite::Store::IN_MEMORY);
	}
}