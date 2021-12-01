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

#include <Game.Character.Action.h>
#include <Game.Island.h>
#include <Game.Quest.h>
#include <Game.Ship.DockResult.h>
namespace state::in_play
{
	int GetPlayerCharacterId();
	std::optional<game::ship::DockResult> Dock();
	std::optional<int> GetPlayerCharacterIslandId();
	std::optional<int> GetPlayerCharacterShipId();
	std::optional<game::Quest> GetPlayerCharacterQuest();
	std::optional<common::XY<double>> GetPlayerCharacterShipLocation();
	std::optional<std::list<game::Island>> GetPlayerCharacterViewableIslands();
	std::optional<double> GetPlayerCharacterShipHeading();
	std::optional<double> GetPlayerCharacterShipSpeed();
	void PlayMainTheme();
	std::list<game::Island> GetPlayerCharacterKnownIslands();
	void SetPlayerCharacterShipHeading(double);
	void DoPlayerCharacterAction(const game::character::Action&);
	void ChangePlayerCharacterMoney(double);
	//game::character::Statistics::ReadMoney(GetPlayerCharacterId())
	//game::islands::DarkAlley::GetAnte(GetPlayerCharacterIslandId().value())
	//game::islands::DarkAlley::GetMinimumWager(GetPlayerCharacterIslandId().value())
	//data::game::Island::Read(GetPlayerCharacterIslandId().value()).value().location
	//game::character::Ship::AvailableTonnage(GetPlayerCharacterId()).value()
	//game::character::Ship::AvailableTonnage(GetPlayerCharacterId()).value()
	//game::islands::Items::GetPurchasePrices(GetPlayerCharacterIslandId().value())
	//game::character::Items::All(GetPlayerCharacterId())
	//game::character::ShipStatistics::CleanHull(GetPlayerCharacterShipId().value(),
}
