#include "Game.h"
#include "Game.Characters.h" //FOR APPLY TURN EFFECTS
#include "Game.Characters.Plights.h" //FOR APPLY TURN EFFECTS
#include "Game.Demigods.h" //FOR APPLY TURN EFFECTS
#include "Game.Fisheries.h" //FOR APPLY TURN EFFECTS
#include "Game.Islands.h" //FOR APPLY TURN EFFECTS
#include "Game.Player.h" //FOR GetPlayerCharacter
#include "Game.Session.h"
#include "Game.Session.Character.h"
#include "Game.Ships.h" //FOR APPLY TURN EFFECTS
#include "Game.World.h" //FOR APPLY TURN EFFECTS
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
		game::Ships::ApplyTurnEffects();
		game::Characters::ApplyTurnEffects();
		game::Islands::ApplyTurnEffects();
		game::characters::Plights::ApplyTurnEffects(game::Player::GetCharacterId());
		game::Demigods::ApplyTurnEffects(game::Player::GetCharacterId());
		game::Fisheries::ApplyTurnEffects();
		game::World::ApplyTurnEffects();
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
		game::ResetLegacy(difficulty);
	}
}