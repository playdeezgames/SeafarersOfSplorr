#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Utility.h>
#include <Game.Audio.Mux.h>
#include <Game.Avatar.Quest.h>
#include <Game.Avatar.Log.h>
#include <Game.Colors.h>
#include "State.InPlay.ConfirmAbandonJob.h"
#include "State.Terminal.h"
#include "UIState.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_CONFIRM_ABANDON_JOB;
	static const std::string LAYOUT_NAME = "State.Terminal";

	static void AbandonJob()
	{
		Terminal::ErrorMessage("You abandon yer job, and yer reputation suffers!");
		game::avatar::Quest::Abandon();
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::RED);
		Terminal::WriteLine("Are you sure you want to abandon yer current job? (You will suffer a reputation penalty!)");

		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine("1) No");
		Terminal::WriteLine("2) Yes");

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
		{ "1", application::UIState::GoTo(::UIState::IN_PLAY_NEXT)},
		{ "2", AbandonJob},
	};

	void ConfirmAbandonJob::Start()
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