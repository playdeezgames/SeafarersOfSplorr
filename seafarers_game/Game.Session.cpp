#include "Game.h"
#include "Game.Characters.h"
#include "Game.Player.h"
#include "Game.Session.h"
#include "Game.Session.Character.h"
#include "Game.Ship.h"
namespace game
{
	std::optional<session::Character> Session::GetCharacter(int characterId) const
	{
		if (game::Characters::Exists(characterId))
		{
			return session::Character(characterId);
		}
		return std::nullopt;
	}

	std::optional<game::session::Character> Session::GetPlayerCharacter() const
	{
		return GetCharacter(game::Player::GetCharacterId());
	}

	std::optional<game::session::Ship> Session::GetShip(int shipId) const
	{
		if (game::Ship::GetShipType(shipId).has_value())
		{
			return game::session::Ship(shipId);
		}
		return std::nullopt;
	}

	session::World Session::GetWorld() const
	{
		return game::session::World();
	}

	void Session::ApplyTurnEffects() const
	{
		game::ApplyTurnEffects();
	}
}