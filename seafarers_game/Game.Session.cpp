#include <Data.SQLite.Store.h> //FOR RESET
#include <Data.SQLite.Stores.h> //FOR RESET
#include <Data.Game.Fishery.h> //For GetFisheries
#include <Data.Game.Island.h> //FOR GetIslands
#include "Game.Demigods.h" //FOR APPLY TURN EFFECTS
#include "Game.Fisheries.h" //FOR APPLY TURN EFFECTS
#include "Game.Session.h"//Cuz we implement here
namespace game
{
	session::Characters Session::GetCharacters() const
	{
		return session::Characters();
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
		GetFisheries().ApplyTurnEffects();
		GetWorld().ApplyTurnEffects();
	}

	session::Items Session::GetItems() const
	{
		return session::Items();
	}

	session::ItemSubtypes Session::GetItemSubtypes() const
	{
		return session::ItemSubtypes();
	}

	void Session::Reset(const Difficulty& difficulty) const
	{
		data::sqlite::Stores::Bounce(data::sqlite::Store::IN_MEMORY);//TODO: mebbe this becomes the new game::Reset?

		GetItemSubtypes().Reset(difficulty);//MUST BE FIRST
		GetWorld().Reset(difficulty);//MUST BE SECOND
		GetShips().Reset(difficulty);
		GetCharacters().Reset(difficulty);
		GetPlayer().Reset(difficulty);
		GetDemigods().Reset(difficulty);
		GetFisheries().Reset(difficulty);
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

	session::Fisheries Session::GetFisheries() const
	{
		auto fisheries = data::game::Fishery::All();
		std::list<int> fisheryIds;
		for (auto fishery : fisheries)
		{
			fisheryIds.push_back(fishery.fisheryId);
		}
		return session::Fisheries([fisheryIds]() { return fisheryIds; });
	}

	session::Saves Session::GetSaves() const
	{
		return session::Saves();
	}

	session::Player Session::GetPlayer() const
	{
		return session::Player();
	}
}