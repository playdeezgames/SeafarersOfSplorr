#include "Game.FishGame.h"
#include "Game.Session.FishGame.h"
namespace game::session
{
	FishGame::FishGame(int characterId)
		: characterId(characterId)
	{

	}

	static std::optional<Fishboard> TryGetBoard(int characterId)
	{
		return Fishboard(characterId);
	}

	Fishboard FishGame::GetBoard() const
	{
		return TryGetBoard(characterId).value();
	}

	static std::optional<bool> TryHasGivenUp(int characterId)
	{
		return game::FishGame::HasGivenUp(characterId);
	}

	bool FishGame::HasGivenUp() const
	{
		return TryHasGivenUp(characterId).value();
	}

	static std::optional<game::FishGameState> TryGetState(int characterId)
	{
		return game::FishGame::GetState(characterId);
	}

	game::FishGameState FishGame::GetState() const
	{
		return TryGetState(characterId).value();
	}

	static std::optional<int> TryGetGuesses(int characterId)
	{
		return game::FishGame::ReadGuesses(characterId);
	}

	int FishGame::GetGuesses() const
	{
		return TryGetGuesses(characterId).value();
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