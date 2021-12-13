#include "Game.FishGame.h"
#include "Game.Session.FishGame.h"
namespace game::session
{
	FishGame::FishGame(int characterId)
		: characterId(characterId)
	{

	}

	std::optional<Fishboard> FishGame::GetBoard() const
	{
		return Fishboard(characterId);
	}

	std::optional<bool> FishGame::HasGivenUp() const
	{
		return game::FishGame::HasGivenUp(characterId);
	}

	std::optional<game::FishGameState> FishGame::GetState() const
	{
		return game::FishGame::GetState(characterId);
	}

	std::optional<int> FishGame::GetGuesses() const
	{
		return game::FishGame::ReadGuesses(characterId);
	}

	void FishGame::GiveUp() const
	{
		game::FishGame::GiveUp(characterId);
	}

	void FishGame::AddBait() const
	{
		game::FishGame::AddBait(characterId);
	}
}