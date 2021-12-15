#include <Data.SQLite.Store.h> //FOR RESET
#include <Data.SQLite.Stores.h> //FOR RESET
#include "Game.Characters.h" //FOR APPLY TURN EFFECTS
#include "Game.Characters.Equipment.h" //FOR RESET
#include "Game.Characters.Items.h" //FOR RESET
#include "Game.Characters.Plights.h" //FOR APPLY TURN EFFECTS
#include "Game.Characters.Statistics.h" //FOR RESET
#include "Game.Demigods.h" //FOR APPLY TURN EFFECTS
#include "Game.Fisheries.h" //FOR APPLY TURN EFFECTS
#include "Game.Islands.h" //FOR APPLY TURN EFFECTS
#include "Game.Islands.Features.h" //FOR RESET
#include "Game.Player.h" //FOR GetPlayerCharacter
#include "Game.Session.h"
#include "Game.Session.Character.h"
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
		GetShips().ApplyTurnEffects();

		game::Characters::ApplyTurnEffects();
		game::Islands::ApplyTurnEffects();
		game::characters::Plights::ApplyTurnEffects(game::Player::GetCharacterId());
		game::Demigods::ApplyTurnEffects(game::Player::GetCharacterId());
		game::Fisheries::ApplyTurnEffects();

		GetWorld().ApplyTurnEffects();
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
		data::sqlite::Stores::Bounce(data::sqlite::Store::IN_MEMORY);//TODO: mebbe this becomes the new game::Reset?

		GetWorld().Reset(difficulty);//MUST BE FIRST
		GetShips().Reset(difficulty);

		game::Characters::Reset(difficulty);
		game::characters::Equipment::Reset(difficulty);
		game::characters::Items::Reset(Player::GetCharacterId(), difficulty);
		game::characters::Plights::Reset(difficulty);
		game::characters::Statistics::Reset(Player::GetCharacterId(), difficulty);
		game::Demigods::Reset(game::Player::GetCharacterId(), difficulty);
		game::Fisheries::Reset(difficulty);
		game::Islands::Reset(difficulty);
		game::islands::Features::Reset(difficulty);
	}
}