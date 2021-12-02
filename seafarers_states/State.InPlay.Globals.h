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

#include <Cards.Rank.h>
#include <Cards.Suit.h>
#include <Game.Character.Action.h>
#include <Game.EquipSlot.h>
#include <Game.Island.h>
#include <Game.Item.h>
#include <Game.Quest.h>
#include <Game.Ship.DockResult.h>
#include <Game.Side.h>
namespace state::in_play
{
	void AddPlayerCharacterMessage(const std::string&, const std::string&);
	void AddPlayerCharacterMessage(const std::string&);
	int GetPlayerCharacterId();
	std::optional<game::ship::DockResult> Dock();//TODO: PlayerCharacterDock?
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
	std::optional<double> ChangePlayerCharacterMoney(double);
	std::optional<common::XY<double>> GetPlayerCharacterIslandLocation();
	std::optional<double> GetPlayerCharacterMoney();
	std::optional<double> GetPlayerCharacterDarkAlleyAnte();
	std::optional<double> GetPlayerCharacterDarkAlleyMinimumWager();
	std::optional<double> GetPlayerCharacterAvailableTonnage();
	std::optional<std::map<game::Item, double>> GetPlayerCharacterPurchasePrices();
	std::map<game::Item, size_t> GetPlayerCharacterItems();
	void PlayerCharacterCleanHull(const game::Side&);

	void SetIslandTradeItem(const game::Item&);
	const game::Item& GetIslandTradeItem();
	void SetEquipmentSlot(const game::EquipSlot&);
	const game::EquipSlot& GetEquipmentSlot();
	void SetCrewDetailCharacterId(int);
	int GetCrewDetailCharacterId();
	const std::map<cards::Rank, std::string>& GetRanks();
	const std::map<cards::Suit, std::string>& GetSuitColors();
	const std::map<cards::Suit, std::string>& GetSuits();
	void SetRenameShipAdjective(const std::string&);
	const std::string& GetRenameShipAdjective();
}
