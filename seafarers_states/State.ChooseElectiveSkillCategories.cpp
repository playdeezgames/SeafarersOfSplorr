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

	static std::set<game::session::SkillCategory> GetFixedSkillCategories()
	{
		return game::Session()
			.GetWorld()
			.GetProfessions()
			.GetProfession(game::session::Player::GetProfession())
			.GetSkillCategories();
	}

	static void RefreshFixedSkillCategories()
	{
		auto fixedCategories = GetFixedSkillCategories();
		std::for_each(
			fixedCategories.begin(),
			fixedCategories.end(),
			[](const game::session::SkillCategory& category)
			{
				Terminal::WriteLine(category.GetName());
			});
	}

	static void RefreshElectiveSkillCategories()
	{
		auto electiveCategories = game::session::Player::GetElectiveSkillCategories();
		std::for_each(
			electiveCategories.begin(),
			electiveCategories.end(),
			[](const game::session::SkillCategory& category)
			{
				Terminal::WriteLine("{}(elective)", category.GetName());
			});
	}

	static size_t GetSkillCategoryCount()
	{
		auto fixedCategories = GetFixedSkillCategories();
		auto electiveCategories = game::session::Player::GetElectiveSkillCategories();
		return fixedCategories.size() + electiveCategories.size();
	}

	static void RefreshRemainingElectiveCount()
	{
		auto skillCategoryCount = GetSkillCategoryCount();
		Terminal::WriteLine("Skill Categories Chosen: {}/{}", skillCategoryCount, game::session::Player::SKILL_CATEGORY_COUNT);
	}


	static void Refresh()
	{
		Terminal::ClearStatusLine();
		Terminal::ClearInput();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine();
		Terminal::WriteLine("Professional Skill Categories:");

		Terminal::SetForeground(game::Colors::GRAY);
		RefreshFixedSkillCategories();
		RefreshElectiveSkillCategories();
		RefreshRemainingElectiveCount();

		Terminal::ShowMenu();

		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine();
		Terminal::Write(">");
	}

	static bool AmIDone()
	{
		return GetSkillCategoryCount() >= game::session::Player::SKILL_CATEGORY_COUNT;
	}

	static void OnDone()
	{
		game::session::Player::ClearProfessionSkillPointAllocations();
		application::UIState::Write(::UIState::ALLOCATE_PROFESSIONAL_SKILL_POINTS);
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

	static std::set<game::session::SkillCategory> GetAlreadyPresentSkillCategories()
	{
		std::set<game::session::SkillCategory> alreadyPresent;
		auto fixedCategories = GetFixedSkillCategories();
		auto electiveCategories = game::session::Player::GetElectiveSkillCategories();
		std::set_union(
			fixedCategories.begin(),
			fixedCategories.end(),
			electiveCategories.begin(),
			electiveCategories.end(),
			std::inserter(alreadyPresent, alreadyPresent.end()));
		return alreadyPresent;
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
			auto alreadyPresent = GetAlreadyPresentSkillCategories();
			auto allCategories = game::Session().GetWorld().GetSkillCategories().GetSkillCategories();
			std::for_each(
				allCategories.begin(),
				allCategories.end(),
				[&alreadyPresent](const game::session::SkillCategory& category)
				{
					if (!alreadyPresent.contains(category))
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