#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Utility.h>
#include <Data.JSON.Stores.h>
#include <format>
#include <Game.Audio.Mux.h>
#include <Game.Avatar.h>
#include <Game.Avatar.Actions.h>
#include <Game.Avatar.Docked.h>
#include <Game.Colors.h>
#include <Game.Islands.h>
#include <Game.Islands.Features.h>
#include "State.InPlay.Docked.h"
#include "State.Terminal.h"
#include "UIState.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_DOCKED;
	static const std::string FORMAT_NAME = "Island Name: {}";
	static const std::string FORMAT_VISITS = "Visits: {}";
	static const std::string FORMAT_TEMPLE = "5) Temple of {}";

	static void Refresh()
	{
		auto location = game::avatar::Docked::ReadLocation().value();
		auto island = game::Islands::Read(location).value();
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Docked:");
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine(FORMAT_NAME, island.name);
		Terminal::WriteLine(FORMAT_VISITS, island.visits.value_or(0));

		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine("1) Jobs");
		Terminal::WriteLine("2) Trade");
		if (game::islands::Features::Read(location, game::Feature::SHIPYARD))
		{
			Terminal::WriteLine("3) Shipyard");
		}
		if (game::islands::Features::Read(location, game::Feature::DARK_ALLEY))
		{
			Terminal::WriteLine("4) Dark Alley");
		}
		Terminal::WriteLine(FORMAT_TEMPLE, island.patronDemigod);
		Terminal::WriteLine("6) Undock");

		Terminal::ShowPrompt();
	}

	static void OnUndock()
	{
		game::avatar::Actions::DoAction(game::avatar::Action::UNDOCK);
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnJob()
	{
		game::avatar::Actions::DoAction(game::avatar::Action::ENTER_JOB_BOARD);
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnTrade()
	{
		game::avatar::Actions::DoAction(game::avatar::Action::ENTER_MARKET);
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnShipyard()
	{
		auto location = game::avatar::Docked::ReadLocation().value();
		if (game::islands::Features::Read(location, game::Feature::SHIPYARD))
		{
			game::avatar::Actions::DoAction(game::avatar::Action::ENTER_SHIPYARD);
			::application::UIState::Write(::UIState::IN_PLAY_NEXT);
		}
		else
		{
			Terminal::ErrorMessage("Please select a valid option.");
			Refresh();
		}
	}

	static void OnDarkAlley()
	{
		auto location = game::avatar::Docked::ReadLocation().value();
		if (game::islands::Features::Read(location, game::Feature::DARK_ALLEY))
		{
			game::avatar::Actions::DoAction(game::avatar::Action::ENTER_DARK_ALLEY);
			::application::UIState::Write(::UIState::IN_PLAY_NEXT);
		}
		else
		{
			Terminal::ErrorMessage("Please select a valid option.");
			Refresh();
		}
	}

	static void OnTemple()
	{
		game::avatar::Actions::DoAction(game::avatar::Action::ENTER_TEMPLE);
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}


	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		Refresh();
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{"1", OnJob},
		{"2", OnTrade},
		{"3", OnShipyard},
		{"4", OnDarkAlley},
		{"5", OnTemple},
		{"6", OnUndock}
	};

	void Docked::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoIntegerInput(
				menuActions,
				"Please enter a number between 1 and 6",
				Refresh));
	}
}