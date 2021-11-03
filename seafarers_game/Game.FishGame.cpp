#include <Common.RNG.h>
#include <Data.Game.FishboardCell.h>
#include <Data.Game.Fishery.h>
#include <Data.Game.FishGame.h>
#include "Game.Avatar.Items.h"
#include "Game.Avatar.Log.h"
#include "Game.Colors.h"
#include "Game.Fishboard.h"
#include "Game.Fisheries.h"
#include "Game.FishGame.h"
#include "Game.Fishes.h"
#include "Game.Items.h"
#include "Game.Player.h"
#include <format>
#include <map>
namespace game//20211013
{
	static const int ADDITIONAL_GUESSES = 5;

	FishGameState FishGame::GetState()
	{
		return
			(Fishboard::IsFullyRevealed()) ? (FishGameState::DONE) :
			(HasGivenUp()) ? (FishGameState::GAVE_UP) :
			(ReadGuesses() > 0) ? (FishGameState::FISHING) :
			(FishGameState::OUT_OF_GUESSES);
	}

	int FishGame::ReadGuesses()
	{
		return data::game::FishGame::ReadGuesses();
	}

	bool FishGame::HasGivenUp()
	{
		return data::game::FishGame::ReadGivenUp();
	}

	void FishGame::GiveUp()
	{
		data::game::FishGame::WriteGuesses(0);
		data::game::FishGame::WriteGivenUp(true);
	}

	bool FishGame::HasGuessesLeft()
	{
		return ReadGuesses() > 0;
	}

	void FishGame::AddBait()
	{
		avatar::Items::Remove(game::Player::GetAvatarId(), Item::BAIT, 1);
		data::game::FishGame::WriteGuesses(data::game::FishGame::ReadGuesses() + ADDITIONAL_GUESSES);
	}
}