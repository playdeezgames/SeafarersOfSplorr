#include <Game.Character.Actions.h>
#include <Game.Character.Quest.h>
#include "State.InPlay.ConfirmReplaceJob.h"
#include "State.InPlay.Globals.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_CONFIRM_REPLACE_JOB;

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::RED);
		Terminal::WriteLine("Are you sure you want to replace yer current job? (You will suffer a reputation penalty!)");

		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine("1) Yes");
		Terminal::WriteLine("0) No");

		Terminal::ShowPrompt();
	}

	static void ReplaceJob()
	{
		Terminal::ErrorMessage("You replace yer job, and yer reputation suffers!");
		game::character::Quest::Abandon(GetPlayerCharacterId());
		game::character::Quest::Accept(GetPlayerCharacterId(), GetPlayerCharacterIslandId().value());
		DoPlayerCharacterAction(game::character::Action::ENTER_DOCK);
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnNo()
	{
		DoPlayerCharacterAction(game::character::Action::ENTER_DOCK);
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnEnter()
	{
		Refresh();
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{ "0", OnNo},
		{ "1", ReplaceJob}
	};

	void ConfirmReplaceJob::Start()
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