#include <Data.SQLite.Store.h>
#include <Data.SQLite.Stores.h>
#include "Game.Session.h"
namespace game
{
	using Stores = data::sqlite::Stores;
	using Store = data::sqlite::Store;

	void Session::ApplyTurnEffects() const
	{
		GetWorld().ApplyTurnEffects();
		GetShips().ApplyTurnEffects();
		GetCharacters().ApplyTurnEffects();
	}

	void Session::Populate(const Difficulty& difficulty) const
	{
		GetWorld().Populate(difficulty);
		GetCharacters().Populate(difficulty);
	}

	void Session::Reset() const
	{
		Stores::Bounce(Store::IN_MEMORY);

		GetWorld().Reset();
		GetShips().Reset();
		GetCharacters().Reset();
		GetPlayer().Reset();
	}
}