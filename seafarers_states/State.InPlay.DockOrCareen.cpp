#include <Game.Character.Actions.h>
#include <Game.Character.ShipStatistics.h>
#include "State.InPlay.DockOrCareen.h"
#include "State.InPlay.Globals.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_DOCK_OR_CAREEN;

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Dock or Careen:");
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine("Port fouling {:.0f}%", game::character::ShipStatistics::GetFoulingPercentage(GetPlayerCharacterShipId().value(), game::Side::PORT));
		Terminal::WriteLine("Starboard fouling {:.0f}%", game::character::ShipStatistics::GetFoulingPercentage(GetPlayerCharacterShipId().value(), game::Side::STARBOARD));

		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine("1) Dock");
		Terminal::WriteLine("2) Careen to port");
		Terminal::WriteLine("3) Careen to starboard");
		Terminal::WriteLine("0) Never mind");

		Terminal::ShowPrompt();
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		Refresh();
	}

	static void CompleteQuest(const game::Quest& quest)
	{
		Terminal::WriteLine("==DELIVERY COMPLETE==");
		Terminal::Write("{} the {} ", quest.personName, quest.professionName);
		Terminal::Write("is {} ", quest.receiptEmotion);
		Terminal::Write("when given the ");
		Terminal::Write("{}. ", quest.itemName);
		Terminal::WriteLine("Yer reputation increases!");
	}

	static void OnDock()
	{
		auto quest = GetPlayerCharacterQuest();//after the call to Dock(), this will be nullopt if completed!
		if (Dock() == game::ship::DockResult::COMPLETED_QUEST)
		{
			CompleteQuest(quest.value());
		}
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnCareenToPort()
	{
		Terminal::WriteLine();
		Terminal::SetForeground(game::Colors::GREEN);
		Terminal::WriteLine("You careen the vessel on its port side.");
		game::character::Actions::DoAction(GetPlayerCharacterId(), game::character::Action::CAREEN_TO_PORT);
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnCareenToStarboard()
	{
		Terminal::WriteLine();
		Terminal::SetForeground(game::Colors::GREEN);
		Terminal::WriteLine("You careen the vessel on its starboard side.");
		game::character::Actions::DoAction(GetPlayerCharacterId(), game::character::Action::CAREEN_TO_STARBOARD);
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{"1", OnDock},
		{"2", OnCareenToPort},
		{"3", OnCareenToStarboard},
		{"0", application::UIState::GoTo(::UIState::IN_PLAY_AT_SEA_OVERVIEW)}
	};

	void DockOrCareen::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoIntegerInput(
				menuActions,
				"Please enter a number between 1 and 4.",
				Refresh));
	}
}