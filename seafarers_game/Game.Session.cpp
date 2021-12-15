#include <Data.SQLite.Store.h> //FOR RESET
#include <Data.SQLite.Stores.h> //FOR RESET
#include <Data.Game.Island.h> //FOR GetIslands
#include "Game.Demigods.h" //FOR APPLY TURN EFFECTS
#include "Game.Fisheries.h" //FOR APPLY TURN EFFECTS
#include "Game.Player.h" //FOR GetPlayerCharacter
#include "Game.Session.h"//Cuz we implement here
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
		GetCharacters().ApplyTurnEffects();
		GetIslands().ApplyTurnEffects();

		GetDemigods().ApplyTurnEffects();

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
		GetCharacters().Reset(difficulty);
		GetDemigods().Reset(difficulty);

		game::Fisheries::Reset(difficulty);

		GetIslands().Reset(difficulty);
	}

	session::Islands Session::GetIslands() const
	{
		auto islands = data::game::Island::All();
		std::list<int> islandIds;
		for (auto island : islands)
		{
			islandIds.push_back(island.id);
		}
		return session::Islands([islandIds]() { return islandIds; });
	}

	session::Demigods Session::GetDemigods() const
	{
		return session::Demigods();
	}

}