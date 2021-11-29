#pragma once
#include "Game.FishGameState.h"
namespace game
{
	struct FishGame
	{
		static int ReadGuesses(int);
		static bool HasGuessesLeft(int);

		static bool HasGivenUp(int);
		static void GiveUp(int);

		static void AddBait(int);

		static FishGameState GetState(int);
	};
}
