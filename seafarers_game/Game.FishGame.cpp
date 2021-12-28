#include <Data.Game.FishGame.h>
#include "Game.Fishboard.h"
#include "Game.FishGame.h"
namespace game
{
	static const int ADDITIONAL_GUESSES = 5;

	FishGameState FishGame::GetState(int characterId)
	{
		return
			(Fishboard::IsFullyRevealed(characterId)) ? (FishGameState::DONE) :
			(HasGivenUp(characterId)) ? (FishGameState::GAVE_UP) :
			(ReadGuesses(characterId) > 0) ? (FishGameState::FISHING) :
			(FishGameState::OUT_OF_GUESSES);
	}

	int FishGame::ReadGuesses(int characterId)
	{
		return data::game::FishGame::ReadGuesses(characterId);
	}

	bool FishGame::HasGivenUp(int characterId)
	{
		return data::game::FishGame::ReadGivenUp(characterId);
	}

	void FishGame::GiveUp(int characterId)
	{
		data::game::FishGame::WriteGuesses(characterId, 0);
		data::game::FishGame::WriteGivenUp(characterId, true);
	}

	bool FishGame::HasGuessesLeft(int characterId)
	{
		return ReadGuesses(characterId) > 0;
	}

	void FishGame::AddBait(int characterId)
	{
		data::game::FishGame::WriteGuesses(characterId, data::game::FishGame::ReadGuesses(characterId) + ADDITIONAL_GUESSES);
	}
}