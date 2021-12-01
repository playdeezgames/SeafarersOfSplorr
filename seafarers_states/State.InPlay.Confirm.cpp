#include <Common.Utility.h>
#include "State.InPlay.Confirm.h"
#include "State.InPlay.Globals.h"
#include <Visuals.Confirmations.h>
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_CONFIRM;

	static visuals::Confirmation confirmation;

	static void OnYes()
	{
		confirmation.onYes();
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnNo()
	{
		confirmation.onNo();
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine(confirmation.prompt);
		Terminal::WriteLine();

		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine("1) No");
		Terminal::WriteLine("2) Yes");

		Terminal::ShowPrompt();
	}

	static void OnEnter()
	{
		if (visuals::Confirmations::HasConfirmation())
		{
			confirmation = visuals::Confirmations::Read();
			Refresh();
			PlayMainTheme();//TODO: have this come from the confirmation object?
			return;
		}
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{ "1", OnNo},
		{ "2", OnYes}
	};

	void Confirm::Start()
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