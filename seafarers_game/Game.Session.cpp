#include "Game.h"
#include "Game.Characters.h"
#include "Game.Player.h"
#include "Game.Session.h"
#include "Game.Session.Character.h"
#include "Game.Ship.h"
namespace game
{
	session::Characters Session::GetCharacters() const
	{
		return session::Characters();
	}

	session::Character Session::GetPlayerCharacter() const
	{
		return GetCharacters().GetCharacter(game::Player::GetCharacterId());
	}

	session::Ships Session::GetShips() const
	{
		return session::Ships();
	}

	session::World Session::GetWorld() const
	{
		return game::session::World();
	}

	void Session::ApplyTurnEffects() const
	{
		game::ApplyTurnEffects();
	}

	session::Items Session::GetItems() const
	{
		return session::Items();
	}

	session::ItemTypes Session::GetItemTypes() const
	{
		return session::ItemTypes();
	}

	void Session::Reset(const Difficulty& difficulty) const
	{
		game::Reset(difficulty);
	}
}