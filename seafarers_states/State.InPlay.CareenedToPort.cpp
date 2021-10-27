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
#include <Game.Avatar.ShipStatistics.h>
#include <Game.Colors.h>
#include <Game.Islands.h>
#include "State.InPlay.CareenedToPort.h"
#include "State.Terminal.h"
#include "UIState.h"
#include <Visuals.Areas.h>
#include <Visuals.MenuItems.h>
#include <Visuals.Menus.h>
#include <Visuals.Texts.h>
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_CAREENED_TO_PORT;
	static const std::string LAYOUT_NAME = "State.Terminal";
	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Careened to port:");
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine("Starboard fouling {:.0f}%", game::avatar::ShipStatistics::GetFoulingPercentage(game::Side::STARBOARD));

		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine("1) Clean hull");
		Terminal::WriteLine("2) Belay");

		Terminal::ShowPrompt();
	}

	static void OnCleanHull()
	{
		Terminal::WriteLine();
		Terminal::SetForeground(game::Colors::GREEN);
		Terminal::WriteLine("You clean the hull.");
		game::avatar::ShipStatistics::CleanHull(game::Side::STARBOARD);
		Refresh();
	}

	static void OnBelay()
	{
		Terminal::WriteLine();
		Terminal::SetForeground(game::Colors::GREEN);
		Terminal::WriteLine("You right the vessel.");
		game::avatar::Actions::DoAction(game::avatar::Action::UNCAREEN);
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		Refresh();
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{"1", OnCleanHull },
		{"2", OnBelay }
	};

	void CareenedToPort::Start()
	{
		::application::OnEnter::AddHandler(
			CURRENT_STATE, 
			OnEnter);
		::application::Renderer::SetRenderLayout(
			CURRENT_STATE, 
			LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoIntegerInput(
				menuActions,
				"Please enter a number between 1 and 2.",
				Refresh));

	}
}