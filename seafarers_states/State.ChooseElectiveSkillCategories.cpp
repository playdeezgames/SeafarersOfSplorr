#include <algorithm>
#include <Game.Session.h>
#include <iterator>
#include <set>
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

		Terminal::SetForeground(game::Colors::GRAY);
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
				Terminal::WriteLine(category.GetName());
			});

		//list electives
		auto electiveCategories = game::session::Player::GetElectiveSkillCategories();
		std::for_each(
			electiveCategories.begin(),
			electiveCategories.end(),
			[](const game::session::SkillCategory& category)
			{
				Terminal::WriteLine("{}(elective)", category.GetName());
			});
		//TODO: you have # of skills categories left to elect

		Terminal::ShowMenu();

		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine();
		Terminal::Write(">");
	}

	static bool AmIDone()
	{
		auto fixedCategories =
			game::Session()
			.GetWorld()
			.GetProfessions()
			.GetProfession(game::session::Player::GetProfession())
			.GetSkillCategories();
		auto electiveCategories = game::session::Player::GetElectiveSkillCategories();
		return fixedCategories.size() + electiveCategories.size() >= game::session::Player::SKILL_CATEGORY_COUNT;
	}

	static void OnDone()
	{

	}

	static void UpdateMenu();

	static std::function<void()> DoElectSkillCategory(const game::session::SkillCategory& category)
	{
		return [category]() 
		{
			game::session::Player::AddElectiveSkillCategory(category.operator game::SkillCategory());
			UpdateMenu();
			Refresh();
		};
	}

	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		if (AmIDone())
		{
			Terminal::menu.AddAction({ "Done", OnDone });
		}
		else
		{
			std::set<game::SkillCategory> iHaveTheseAlready;
			auto fixedCategories =
				game::Session()
				.GetWorld()
				.GetProfessions()
				.GetProfession(game::session::Player::GetProfession())
				.GetSkillCategories();
			std::transform(
				fixedCategories.begin(),
				fixedCategories.end(),
				std::inserter(iHaveTheseAlready, iHaveTheseAlready.end()),
				[](const game::session::SkillCategory& category) 
				{
					return category.operator game::SkillCategory();
				});
			auto electiveCategories = game::session::Player::GetElectiveSkillCategories();
			std::transform(
				electiveCategories.begin(),
				electiveCategories.end(),
				std::inserter(iHaveTheseAlready, iHaveTheseAlready.end()),
				[](const game::session::SkillCategory& category)
				{
					return category.operator game::SkillCategory();
				});
			auto allCategories = game::Session().GetWorld().GetSkillCategories().GetSkillCategories();
			std::for_each(
				allCategories.begin(),
				allCategories.end(),
				[&iHaveTheseAlready](const game::session::SkillCategory& category) 
				{
					if (!iHaveTheseAlready.contains(category.operator game::SkillCategory()))
					{
						Terminal::menu.AddAction({ category.GetName(), DoElectSkillCategory(category) });
					}
				});
		}
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