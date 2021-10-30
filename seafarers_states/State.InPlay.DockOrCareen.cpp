#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Game.Audio.Mux.h>
#include <Game.Avatar.Actions.h>
#include <Game.Avatar.Docked.h>
#include <Game.Avatar.Quest.h>
#include <Game.Avatar.ShipStatistics.h>
#include <Game.Colors.h>
#include <Game.Ship.h>
#include "State.InPlay.DockOrCareen.h"
#include "State.Terminal.h"
#include "UIState.h"
#include <Visuals.Messages.h>
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_DOCK_OR_CAREEN;
	static const std::string LAYOUT_NAME = "State.Terminal";

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Dock or Careen:");
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine("Port fouling {:.0f}%", game::avatar::ShipStatistics::GetFoulingPercentage(game::Side::PORT));
		Terminal::WriteLine("Starboard fouling {:.0f}%", game::avatar::ShipStatistics::GetFoulingPercentage(game::Side::STARBOARD));

		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine("1) Dock");
		Terminal::WriteLine("2) Careen to port");
		Terminal::WriteLine("3) Careen to starboard");
		Terminal::WriteLine("4) Never mind");

		Terminal::ShowPrompt();
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		Refresh();
	}

	static void OnDock()
	{
		auto quest = game::avatar::Quest::Read();
		if (game::avatar::Docked::Dock() == game::avatar::DockResult::COMPLETED_QUEST)
		{
			visuals::Messages::Write(
			{
				"==DELIVERY COMPLETE==",
				{
					{
						{19,5},
						std::format("{} the {} ",quest.value().personName, quest.value().professionName),
						game::Colors::GRAY,
						visuals::HorizontalAlignment::CENTER
					},
					{
						{19,7},
						std::format("is {} ",quest.value().receiptEmotion),
						game::Colors::GRAY,
						visuals::HorizontalAlignment::CENTER
					},
					{
						{19,9},
						"when given the ",
						game::Colors::GRAY,
						visuals::HorizontalAlignment::CENTER
					},
					{
						{19,11},
						std::format("{}. ",quest.value().itemName),
						game::Colors::GRAY,
						visuals::HorizontalAlignment::CENTER
					},
					{
						{19,14},
						"Yer reputation increases!",
						game::Colors::GRAY,
						visuals::HorizontalAlignment::CENTER
					}
				}
			});
		}
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnCareenToPort()
	{
		Terminal::WriteLine();
		Terminal::SetForeground(game::Colors::GREEN);
		Terminal::WriteLine("You careen the vessel on its port side.");
		game::avatar::Actions::DoAction(game::avatar::Action::CAREEN_TO_PORT);
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnCareenToStarboard()
	{
		Terminal::WriteLine();
		Terminal::SetForeground(game::Colors::GREEN);
		Terminal::WriteLine("You careen the vessel on its starboard side.");
		game::avatar::Actions::DoAction(game::avatar::Action::CAREEN_TO_STARBOARD);
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{"1", OnDock},
		{"2", OnCareenToPort},
		{"3", OnCareenToStarboard},
		{"4", application::UIState::GoTo(::UIState::IN_PLAY_AT_SEA_OVERVIEW)}
	};

	void DockOrCareen::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoIntegerInput(
				menuActions,
				"Please enter a number between 1 and 4.",
				Refresh));
	}
}