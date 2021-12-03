#include "State.InPlay.Globals.h"
#include "State.InPlay.Tavern.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_TAVERN;

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Yer in a tavern.");

		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine("0) Leave");

		Terminal::ShowPrompt();

	}

	static void OnLeave()
	{
		DoPlayerCharacterAction(game::character::Action::ENTER_DOCK);
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnEnter()
	{
		PlayMainTheme();
		Refresh();
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{ "0", OnLeave }
	};

	void Tavern::Start()
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
