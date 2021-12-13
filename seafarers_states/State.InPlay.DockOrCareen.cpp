#include "State.InPlay.DockOrCareen.h"
#include "State.InPlay.Globals.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_DOCK_OR_CAREEN;

	static void Refresh()
	{
		Terminal::Reinitialize();

		auto ship = 
			GetGameSession()
			.GetPlayerCharacter()
			.GetBerth().value()
			.GetShip().value();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Dock or Careen:");
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine("Port fouling {:.0f}%",
			ship.GetFouling(game::Side::PORT).GetPercentage());
		Terminal::WriteLine("Starboard fouling {:.0f}%", 
			ship.GetFouling(game::Side::STARBOARD).GetPercentage());

		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine("1) Dock");
		Terminal::WriteLine("2) Careen to port");
		Terminal::WriteLine("3) Careen to starboard");
		Terminal::WriteLine("0) Never mind");

		Terminal::ShowPrompt();
	}

	static void OnEnter()
	{
		PlayMainTheme();
		Refresh();
	}

	static void CompleteQuest(const std::string& message)
	{
		auto playerCharacter =
			GetGameSession()
			.GetPlayerCharacter();
		playerCharacter.GetMessages().Add(game::Colors::LIGHT_CYAN, "Delivery Complete!");
		playerCharacter.GetMessages().Add(game::Colors::GRAY, message);
		playerCharacter.GetMessages().Add(game::Colors::GREEN,"Yer reputation increases!");//<-
	}

	static std::optional<std::string> GetQuestCompletionMesssage()
	{
		auto quest = GetGameSession().GetPlayerCharacter().GetQuest();
		if (quest)
		{
			return quest.value().GetCompletionMessage();
		}
		return std::nullopt;
	}

	static void OnDock()
	{
		auto message = GetQuestCompletionMesssage();
		if (Dock() == game::ship::DockResult::COMPLETED_QUEST)
		{
			CompleteQuest(message.value());
		}
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnCareenToPort()
	{
		auto playerCharacter =
			GetGameSession()
				.GetPlayerCharacter();
		playerCharacter.GetMessages().Add(game::Colors::GREEN,"You careen the vessel on its port side.");
		playerCharacter.DoAction(game::characters::Action::CAREEN_TO_PORT);
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnCareenToStarboard()
	{
		auto playerCharacter =
			GetGameSession()
			.GetPlayerCharacter();
		playerCharacter.GetMessages().Add(game::Colors::GREEN, "You careen the vessel on its starboard side.");
		playerCharacter.DoAction(game::characters::Action::CAREEN_TO_STARBOARD);
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
				Terminal::INVALID_INPUT,
				Refresh));
	}
}