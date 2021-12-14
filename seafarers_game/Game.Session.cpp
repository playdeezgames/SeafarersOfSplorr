#include "Game.h"
#include "Game.Characters.h"
#include "Game.Player.h"
#include "Game.Session.h"
#include "Game.Session.Character.h"
#include "Game.Ship.h"
namespace game
{
	static std::optional<session::Character> TryGetCharacter(int characterId)
	{
		if (game::Characters::Exists(characterId))
		{
			return session::Character(characterId);
		}
		return std::nullopt;
	}

	session::Character Session::GetCharacter(int characterId) const
	{
		return TryGetCharacter(characterId).value();
	}

	static std::optional<game::session::Character> TryGetPlayerCharacter()
	{
		return TryGetCharacter(game::Player::GetCharacterId());
	}

	session::Character Session::GetPlayerCharacter() const
	{
		return TryGetPlayerCharacter().value();
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