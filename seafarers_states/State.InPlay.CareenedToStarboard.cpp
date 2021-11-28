#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Utility.h>
#include <Data.JSON.Stores.h>
#include <format>
#include <Game.Audio.Mux.h>
#include <Game.Character.h>
#include <Game.Character.Actions.h>
#include <Game.Character.Docked.h>
#include <Game.Character.ShipStatistics.h>
#include <Game.Colors.h>
#include <Game.Islands.h>
#include <Game.Player.h>
#include "State.InPlay.CareenedToStarboard.h"
#include "State.Terminal.h"
#include "UIState.h"
#include <Visuals.Areas.h>
#include <Visuals.MenuItems.h>
#include <Visuals.Menus.h>
#include <Visuals.Texts.h>
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_CAREENED_TO_STARBOARD;
	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Careened to starboard:");
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine("Port fouling {:.0f}%", game::character::ShipStatistics::GetFoulingPercentage(game::Side::PORT));

		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine("1) Clean port side");
		Terminal::WriteLine("0) Right the vessel");

		Terminal::ShowPrompt();
	}

	static void OnCleanHull()
	{
		Terminal::WriteLine();
		Terminal::SetForeground(game::Colors::GREEN);
		Terminal::WriteLine("You clean the port side.");
		game::character::ShipStatistics::CleanHull(game::Side::PORT);
		Refresh();
	}

	static void OnBelay()
	{
		Terminal::WriteLine();
		Terminal::SetForeground(game::Colors::GREEN);
		Terminal::WriteLine("You right the vessel.");
		game::character::Actions::DoAction(game::Player::GetCharacterId(), game::character::Action::UNCAREEN);
		application::UIState::Write(::UIState::IN_PLAY_DOCK_OR_CAREEN);
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		Refresh();
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{"1", OnCleanHull },
		{"0", OnBelay }
	};

	void CareenedToStarboard::Start()
	{
		::application::OnEnter::AddHandler(
			CURRENT_STATE,
			OnEnter);
		::application::Renderer::SetRenderLayout(
			CURRENT_STATE,
			Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoIntegerInput(
				menuActions,
				"Please enter a number between 1 and 2.",
				Refresh));

	}
}