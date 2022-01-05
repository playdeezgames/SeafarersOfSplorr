#pragma once
#include <Application.Keyboard.h> //common include
#include <Application.OnEnter.h> //common include
#include <Application.Renderer.h> //common include
#include <Application.UIState.h> //common include
#include <Game.Audio.Mux.h> //common include
#include <Game.Audio.Sfx.h> //common include
#include <Game.Colors.h> //common include
#include "State.Terminal.h" //common include
#include "UIState.h" //common include
namespace state::in_play
{
	void PlayMainTheme();
	void SetCrewDetailCharacterId(int characterId);
	int GetCrewDetailCharacterId();
	void SetRenameShipAdjective(const std::string& adjective);
	const std::string& GetRenameShipAdjective();
	void SetHeadForMark(const std::string& markName);
	const std::string GetHeadForMark();
}
