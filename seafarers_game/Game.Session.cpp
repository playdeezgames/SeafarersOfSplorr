#include <Data.SQLite.Store.h> //FOR RESET
#include <Data.SQLite.Stores.h> //FOR RESET
#include <Data.Game.Island.h> //FOR GetIslands
#include "Game.Session.h"//Cuz we implement here
namespace game
{
	void Session::ApplyTurnEffects() const
	{
		GetShips().ApplyTurnEffects();
		GetCharacters().ApplyTurnEffects();
		GetWorld().ApplyTurnEffects();
	}

	void Session::Populate(const Difficulty& difficulty) const
	{
		GetWorld().Populate(difficulty);
	}

	void Session::Reset() const
	{
		data::sqlite::Stores::Bounce(data::sqlite::Store::IN_MEMORY);//TODO: mebbe this becomes the new game::Reset?

		GetWorld().Reset();
		GetShips().Reset();
		GetCharacters().Reset();
		GetPlayer().Reset();
	}
}