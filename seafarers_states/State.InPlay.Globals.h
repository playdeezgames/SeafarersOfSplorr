#pragma once
#include <Application.Keyboard.h> //common include
#include <Application.OnEnter.h> //common include
#include <Application.Renderer.h> //common include
#include <Application.UIState.h> //common include
#include <Game.Audio.Mux.h> //common include
#include <Game.Audio.Sfx.h> //common include
#include <Game.Colors.h> //common include
#include <Game.Difficulty.h>
#include <Game.Profession.h>
#include "State.Terminal.h" //common include
#include "UIState.h" //common include
namespace state::in_play
{
	void PlayMainTheme();

	//these have to do with starting a new game, and are not needed once the game has been started
	void SetGameDifficulty(const game::Difficulty& difficulty);//the world is populated... so this is associated with the world...
	game::Difficulty GetGameDifficulty();
	void SetProfession(const game::Profession& profession);
	game::Profession GetProfession();
}
