#pragma once
#include "Game.FishGameState.h"
#include "Game.Session.Fishboard.h"
#include <optional>
namespace game::session
{
	struct FishGame
	{
		FishGame(int);
		Fishboard GetBoard() const;
		bool HasGivenUp() const;
		game::FishGameState GetState() const;
		int GetGuesses() const;
		void GiveUp() const;
		void AddBait() const;
	private:
		int characterId;
	};
}
