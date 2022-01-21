#include <Data.SQLite.Store.h>
#include <Data.SQLite.Stores.h>
#include "Game.Session.h"
#include "Game.Session.Player.h"
#include "Game.Session.Ships.h"
#include "Game.Session.World.h"
#include "Game.Session.Characters.h"
namespace game
{
	using Stores = data::sqlite::Stores;
	using Store = data::sqlite::Store;

	void Session::ApplyTurnEffects() const
	{
		game::session::World().ApplyTurnEffects();
		game::session::Ships().ApplyTurnEffects();
		game::session::Characters().ApplyTurnEffects();
	}

	void Session::Populate(const Difficulty& difficulty) const
	{
		game::session::World().Populate(difficulty);
		game::session::Characters().Populate(difficulty);
	}

	void Session::Reset() const
	{
		Stores::Bounce(Store::IN_MEMORY);

		game::session::World().Reset();
		game::session::Ships().Reset();
		game::session::Characters().Reset();
		game::session::Player().Reset();
	}
}