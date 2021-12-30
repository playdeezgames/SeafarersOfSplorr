#pragma once
#include <Application.Keyboard.h> //common include
#include <Application.OnEnter.h> //common include
#include <Application.Renderer.h> //common include
#include <Application.UIState.h> //common include
#include <Common.XY.h>
#include <Game.Audio.Mux.h> //common include
#include <Game.Audio.Sfx.h> //common include
#include <Game.Colors.h> //common include
#include "State.Terminal.h" //common include
#include "UIState.h" //common include

#include <Cards.Rank.h>
#include <Cards.Suit.h>
#include <Game.Characters.Action.h>
#include <Game.Side.h>
namespace state::in_play
{
	void AddPlayerCharacterMessage(const std::string&, const std::string&);
	void AddPlayerCharacterMessage(const std::string&);
	template<typename... Ts>
	static void AddPlayerCharacterMessage(const std::string& color, const std::string& fmt, Ts... args)
	{
		AddPlayerCharacterMessage(color, std::format(fmt, args...));
	}
	int GetPlayerCharacterId();
	std::optional<int> GetPlayerCharacterShipId();
	std::optional<common::XY<double>> GetPlayerCharacterShipLocation();
	std::optional<double> GetPlayerCharacterShipHeading();
	std::optional<double> GetPlayerCharacterShipSpeed();
	void PlayMainTheme();
	void SetPlayerCharacterShipHeading(double);
	void DoPlayerCharacterAction(const game::characters::Action&);

	void SetCrewDetailCharacterId(int);
	int GetCrewDetailCharacterId();
	const std::map<cards::Rank, std::string>& GetRanks();
	const std::map<cards::Suit, std::string>& GetSuitColors();
	const std::map<cards::Suit, std::string>& GetSuits();
	void SetRenameShipAdjective(const std::string&);
	const std::string& GetRenameShipAdjective();
}
