#pragma once
#include <Common.XY.h>
#include "Game.Fish.h"
#include "Game.FishboardCell.h"
#include "Game.FishGameState.h"
#include <optional>
namespace game//20211013
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
