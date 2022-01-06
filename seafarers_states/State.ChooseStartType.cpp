#include <Game.Session.h>
#include "State.InPlay.Globals.h"
#include "State.Terminal.h"
#include "State.ChooseStartType.h"
#include "UIState.h"
namespace state
{
	static const ::UIState CURRENT_STATE = ::UIState::CHOOSE_START_TYPE;

	static void Refresh()
	{
		Terminal::ClearStatusLine();
		Terminal::ClearInput();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine();
		Terminal::WriteLine("Choose Start Type:");

		Terminal::ShowMenu();

		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine();
		Terminal::Write(">");
	}

	static void OnQuickStart()
	{
		game::session::Player::SetCharacterCreationDefaults();
		//TODO: feed in the preselected profession and skills here
		game::Session().GetPlayer().Populate(game::Session().GetWorld().GetDifficulty());
		application::UIState::Write(::UIState::TIP);
	}

	static void OnDetailedStart()
	{
		application::UIState::Write(::UIState::DETAILED_START);
	}

	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		Terminal::menu.AddAction({ "Quick Start", OnQuickStart });
		Terminal::menu.AddAction({ "Detailed Start", OnDetailedStart });
		MenuAction defaultAction = { "Never mind", application::UIState::GoTo(::UIState::START_GAME) };
		Terminal::menu.SetDefaultAction(defaultAction);
	}

	static void OnEnter()
	{
		in_play::PlayMainTheme();
		UpdateMenu();
		Refresh();
	}

	void ChooseStartType::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoMenuInput(
				Terminal::INVALID_INPUT,
				Refresh));
	}
}