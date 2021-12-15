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

	static std::optional<game::session::Ship> TryGetShip(int shipId)
	{
		if (game::Ship::GetShipType(shipId).has_value())
		{
			return game::session::Ship(shipId);
		}
		return std::nullopt;
	}

	session::Ship Session::GetShip(int shipId) const
	{
		return TryGetShip(shipId).value();
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
}