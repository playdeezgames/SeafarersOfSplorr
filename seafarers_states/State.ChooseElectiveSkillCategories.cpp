#include <algorithm>
#include <Game.Session.h>
#include "State.InPlay.Globals.h"
#include "State.Terminal.h"
#include "State.ChooseElectiveSkillCategories.h"
#include "UIState.h"
namespace state
{
	static const ::UIState CURRENT_STATE = ::UIState::CHOOSE_ELECTIVE_SKILL_CATEGORIES;

	static void Refresh()
	{
		Terminal::ClearStatusLine();
		Terminal::ClearInput();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine();
		Terminal::WriteLine("Professional Skill Categories:");
		
		auto fixedCategories = 
			game::Session()
			.GetWorld()
			.GetProfessions()
			.GetProfession(game::session::Player::GetProfession())
			.GetSkillCategories();
		std::for_each(
			fixedCategories.begin(),
			fixedCategories.end(),
			[](const game::session::SkillCategory& category) 
			{
				//Terminal::WriteLine(category.GetName());
			});

		//list electives

		Terminal::ShowMenu();

		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine();
		Terminal::Write(">");
	}

	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		//Terminal::menu.AddAction({ "", OnQuickStart });
		MenuAction defaultAction = { "Never mind", application::UIState::GoTo(::UIState::DETAILED_START) };
		Terminal::menu.SetDefaultAction(defaultAction);
	}

	static void OnEnter()
	{
		in_play::PlayMainTheme();
		UpdateMenu();
		Refresh();
	}

	void ChooseElectiveSkillCategories::Start()
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