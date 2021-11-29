#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Utility.h>
#include <Game.Audio.Mux.h>
#include <Game.Character.Quest.h>
#include <Game.Colors.h>
#include <Game.Player.h>
#include "State.InPlay.ConfirmAbandonJob.h"
#include "State.InPlay.Globals.h"
#include "State.Terminal.h"
#include "UIState.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_CONFIRM_ABANDON_JOB;

	static void AbandonJob()
	{
		Terminal::ErrorMessage("You abandon yer job, and yer reputation suffers!");
		game::character::Quest::Abandon(GetPlayerCharacterId());
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::RED);
		Terminal::WriteLine("Are you sure you want to abandon yer current job? (You will suffer a reputation penalty!)");

		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine("1) Yes");
		Terminal::WriteLine("0) No");

		Terminal::ShowPrompt();
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(
			game::audio::Theme::MAIN);
		Refresh();
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{ "0", application::UIState::GoTo(::UIState::IN_PLAY_NEXT)},
		{ "1", AbandonJob},
	};

	void ConfirmAbandonJob::Start()
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
				Terminal::INVALID_INPUT,
				Refresh));
	}
}