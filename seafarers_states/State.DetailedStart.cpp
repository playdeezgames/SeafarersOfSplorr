#include <algorithm>
#include <Game.Session.h>
#include "State.InPlay.Globals.h"
#include "State.Terminal.h"
#include "State.DetailedStart.h"
#include "UIState.h"
namespace state
{
	static const ::UIState CURRENT_STATE = ::UIState::DETAILED_START;

	static void Refresh()
	{
		Terminal::ClearStatusLine();
		Terminal::ClearInput();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine();
		Terminal::WriteLine("Choose Profession:");

		Terminal::ShowMenu();

		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine();
		Terminal::Write(">");
	}

	static std::function<void()> DoSelectProfession(const game::Profession& profession)
	{
		return [profession]()
		{
			game::session::Player::SetProfession(profession);
			//set skill categories for further character creation
			//go to next step of character creation
		};
	}

	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		auto professions = game::Session().GetWorld().GetProfessions().GetProfessions();
		std::for_each(
			professions.begin(),
			professions.end(),
			[](const game::session::world::Profession& profession) 
			{
				Terminal::menu.AddAction({ profession.GetName(), DoSelectProfession(profession.operator game::Profession()) });
			});
		MenuAction defaultAction = { "Never mind", application::UIState::GoTo(::UIState::CHOOSE_START_TYPE) };
		Terminal::menu.SetDefaultAction(defaultAction);
	}

	static void OnEnter()
	{
		in_play::PlayMainTheme();
		UpdateMenu();
		Refresh();
	}

	void DetailedStart::Start()
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