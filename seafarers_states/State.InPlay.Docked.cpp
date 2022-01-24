#include "State.InPlay.Docked.h"
#include "State.InPlay.Globals.h"
#include "State.InPlay.IslandDistrict.h"
#include "State.InPlay.Next.h"
#include "State.ScratchPad.IslandDistrict.h"
#include <algorithm>
#include <Game.Session.Player.h>
#include <Game.Session.Character.h>
#include <Game.Session.Character.Berth.h>
#include <Game.Session.Island.h>
#include <Game.Session.Island.Districts.h>
#include <Game.Session.Ship.h>
namespace state::in_play
{
	std::optional<int> Docked::stateId = std::nullopt;
	static const std::string FORMAT_NAME = "Island Name: {}";

	static void Refresh()
	{
		auto islandId = game::session::Character(game::session::Player::GetCharacterId()).GetIslandId();
		auto island = game::session::Island(islandId);
		Terminal::Reinitialize();
		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Docked:");
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine(FORMAT_NAME, island.GetName());
		Terminal::ShowMenu();
		Terminal::ShowPrompt();
	}

	static void OnUndock()
	{
		game::session::Ship(
		game::session::character::Berth(game::session::Player::GetCharacterId()).GetShipId())
			.Undock();
		application::UIState::Write(Next::GetStateId());
	}

	static std::function<void()> DoGoToDistrict(const game::session::island::District& district)
	{
		return [district]() 
		{
			scratch_pad::IslandDistrict::SetDistrict(district.operator game::island::District());
			application::UIState::Write(IslandDistrict::GetStateId());
		};
	}

	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		auto districts = 
			game::session::island::Districts(
				game::session::Character(
					game::session::Player::GetCharacterId())
				.GetIslandId())
			.GetDistricts();
		std::for_each(
			districts.begin(),
			districts.end(),
			[](const game::session::island::District& district) 
			{
				Terminal::menu.AddAction({ district.GetName(), DoGoToDistrict(district) });
			});
		MenuAction defaultAction = { "Undock", OnUndock };
		Terminal::menu.SetDefaultAction(defaultAction);
	}

	static void OnEnter()
	{
		PlayMainTheme();
		UpdateMenu();
		Refresh();
	}

	void Docked::Start()
	{
		Registrar::Register(stateId, [](int currentState)
			{
				::application::OnEnter::AddHandler(currentState, OnEnter);
				::application::Renderer::SetRenderLayout(currentState, Terminal::LAYOUT_NAME);
				::application::Keyboard::AddHandler(
					currentState,
					Terminal::DoMenuInput(
						Terminal::INVALID_INPUT,
						Refresh));
			});
	}
}