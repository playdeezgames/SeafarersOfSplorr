#include <Data.SQLite.Store.h>
#include <Data.SQLite.Stores.h>
#include "Game.Session.h"
#include "Game.Session.Characters.h"
#include "Game.Session.Ships.h"
namespace game
{
	using Stores = data::sqlite::Stores;
	using Store = data::sqlite::Store;

	void Session::ApplyTurnEffects() const
	{
		GetWorld().ApplyTurnEffects();
		game::session::Ships().ApplyTurnEffects();
		game::session::Characters().ApplyTurnEffects();
	}

	void Session::Populate(const Difficulty& difficulty) const
	{
		GetWorld().Populate(difficulty);
		game::session::Characters().Populate(difficulty);
	}

	void Session::Reset() const
	{
		Stores::Bounce(Store::IN_MEMORY);

		GetWorld().Reset();
		game::session::Ships().Reset();
		game::session::Characters().Reset();
		GetPlayer().Reset();
	}
}