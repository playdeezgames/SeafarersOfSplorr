#include <Data.SQLite.Store.h> //FOR RESET
#include <Data.SQLite.Stores.h> //FOR RESET
#include <Data.Game.Fishery.h> //For GetFisheries
#include <Data.Game.Island.h> //FOR GetIslands
#include "Game.Fisheries.h" //FOR APPLY TURN EFFECTS
#include "Game.Session.h"//Cuz we implement here
namespace game
{
	session::Characters Session::GetCharacters() const
	{
		return session::Characters();
	}

	void Session::ApplyTurnEffects() const
	{
		GetShips().ApplyTurnEffects();
		GetCharacters().ApplyTurnEffects();
		GetFisheries().ApplyTurnEffects();
		GetWorld().ApplyTurnEffects();
	}

	session::Items Session::GetItems() const
	{
		return session::Items();
	}

	void Session::Populate(const Difficulty& difficulty) const
	{
		GetWorld().Populate(difficulty);
		GetPlayer().Populate(difficulty);
		GetFisheries().Populate(difficulty);
	}

	void Session::Reset() const
	{
		data::sqlite::Stores::Bounce(data::sqlite::Store::IN_MEMORY);//TODO: mebbe this becomes the new game::Reset?

		GetWorld().Reset();
		GetTribes().Reset();
		GetShips().Reset();
		GetCharacters().Reset();
		GetFisheries().Reset();
		GetPlayer().Reset();
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

	session::Tribes Session::GetTribes() const
	{
		return session::Tribes();
	}
}