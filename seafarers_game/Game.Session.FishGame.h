#pragma once
#include "Game.FishGameState.h"
#include "Game.Session.Fishboard.h"
#include <optional>
namespace game::session
{
	struct FishGame
	{
		FishGame(int);
		std::optional<Fishboard> GetBoard() const;
		std::optional<bool> HasGivenUp() const;
		std::optional<game::FishGameState> GetState() const;
		std::optional<int> GetGuesses() const;
		void GiveUp() const;
		void AddBait() const;
	private:
		int characterId;
	};
}
