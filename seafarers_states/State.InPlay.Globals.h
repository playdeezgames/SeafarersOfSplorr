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
#include <Game.Item.h>
#include <Game.Quest.h>
#include <Game.Ship.DockResult.h>
#include <Game.Side.h>
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
	std::optional<common::XY<double>> GetPlayerCharacterIslandLocation();
	double GetPlayerCharacterMoney();
	std::optional<double> GetPlayerCharacterDarkAlleyAnte();
	std::optional<double> GetPlayerCharacterDarkAlleyMinimumWager();
	std::optional<double> GetPlayerCharacterAvailableTonnage();
	std::optional<std::map<game::Item, double>> GetPlayerCharacterPurchasePrices();
	std::map<game::Item, size_t> GetPlayerCharacterItems();
	void PlayerCharacterCleanHull(const game::Side&);
	void SetIslandTradeItem(const game::Item&);
	const game::Item& GetIslandTradeItem();
	//EquipmentSlot::SetSlot(
	//EquipmentSlot::GetSlot()
	//CrewDetail::SetCharacterId(
	//CrewDetail::GetCharacterId()
	//DarkAlleyEntrance::Ranks
	//DarkAlleyEntrance::SuitColors
	//DarkAlleyEntrance::Suits
	//RenameShipAdjective::Read()
}
