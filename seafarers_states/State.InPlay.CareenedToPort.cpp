#include <Game.Character.Actions.h>
#include <Game.Character.ShipStatistics.h>
#include "State.InPlay.CareenedToPort.h"
#include "State.InPlay.Globals.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_CAREENED_TO_PORT;
	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Careened to port:");
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine("Starboard fouling {:.0f}%", 
			game::character::ShipStatistics::GetFoulingPercentage(
				GetPlayerCharacterShipId().value(),
				game::Side::STARBOARD));

		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine("1) Clean starboard side");
		Terminal::WriteLine("0) Right the vessel");

		Terminal::ShowPrompt();
	}

	static void OnCleanHull()
	{
		Terminal::WriteLine();
		Terminal::SetForeground(game::Colors::GREEN);
		Terminal::WriteLine("You clean the starboard side.");
		game::character::ShipStatistics::CleanHull(
			GetPlayerCharacterShipId().value(),
			game::Side::STARBOARD);
		Refresh();
	}

	static void OnBelay()
	{
		Terminal::WriteLine();
		Terminal::SetForeground(game::Colors::GREEN);
		Terminal::WriteLine("You right the vessel.");
		game::character::Actions::DoAction(GetPlayerCharacterId(), game::character::Action::UNCAREEN);
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

	void CareenedToPort::Start()
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