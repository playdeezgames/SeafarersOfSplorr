#include "State.InPlay.Globals.h"
namespace state::in_play
{
	void PlayMainTheme()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
	}

	static game::Difficulty gameDifficulty;

	void SetGameDifficulty(const game::Difficulty& difficulty)
	{
		gameDifficulty = difficulty;
	}

	game::Difficulty GetGameDifficulty()
	{
		return gameDifficulty;
	}
}
