#pragma once
#include "Game.FishGameState.h"
namespace game
{
	struct FishGame
	{
		static int ReadGuesses();
		static bool HasGuessesLeft();

		static bool HasGivenUp();
		static void GiveUp();

		static void AddBait();

		static FishGameState GetState();
	};
}
