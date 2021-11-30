#pragma once
#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Game.Audio.Mux.h>
#include <Game.Audio.Sfx.h>
#include <Game.Colors.h>
#include <Game.Quest.h>
#include <Game.Ship.DockResult.h>
#include "State.Terminal.h"
#include "UIState.h"
namespace state::in_play
{
	int GetPlayerCharacterId();
	std::optional<game::ship::DockResult> Dock();
	std::optional<int> GetPlayerCharacterIslandId();
	std::optional<int> GetPlayerCharacterShipId();
	std::optional<game::Quest> GetPlayerCharacterQuest();
	//GetViewableIslands (5 places in 3 files)
	//game::Ship::GetLocation(GetPlayerCharacterShipId().value()) (4 places in 4 files)
	//game::Ship::GetLocation(GetPlayerCharacterShipId().value()) (4 places in 3 files)
	//game::Ship::GetSpeed(GetPlayerCharacterShipId().value()) (3 places in 3 files)
}
