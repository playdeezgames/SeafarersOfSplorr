#include "State.ChooseElectiveSkillCategories.h"
#include "State.ChooseProfessionalSkill.h"
#include "State.DetailedStart.h"
#include "State.InPlay.Globals.h"
#include "State.ScratchPad.DetailedStart.ElectiveSkillCategories.h"
#include "State.ScratchPad.DetailedStart.Profession.h"
#include "State.ScratchPad.DetailedStart.ProfessionalSkillPointAllocations.h"
#include "State.ScratchPad.DetailedStart.ProfessionalSkillSet.h"
#include <algorithm>
#include <Game.Session.SkillCategories.h>
#include <Game.Session.World.Professions.h>
namespace state
{
	std::optional<int> ChooseElectiveSkillCategories::stateId = std::nullopt;

	static std::set<game::SkillCategory> GetFixedSkillCategories()
	{
		return game::session::world::Professions::GetProfession(
			scratch_pad::detailed_start::Profession::GetProfession()).GetSkillCategories();
	}

	static void RefreshFixedSkillCategories()
	{
		auto fixedCategories = GetFixedSkillCategories();
		std::for_each(
			fixedCategories.begin(),
			fixedCategories.end(),
			[](const auto& category)
			{
				Terminal::WriteLine(game::session::SkillCategory(category).GetName());
			});
	}

	static void RefreshElectiveSkillCategories()
	{
		auto electiveCategories = scratch_pad::detailed_start::ElectiveSkillCategories::GetCategories();
		std::for_each(
			electiveCategories.begin(),
			electiveCategories.end(),
			[](const auto& category)
			{
				Terminal::WriteLine("{}(elective)", game::session::SkillCategory(category).GetName());
			});
	}

	static size_t GetSkillCategoryCount()
	{
		auto fixedCategories = GetFixedSkillCategories();
		auto electiveCategories = scratch_pad::detailed_start::ElectiveSkillCategories::GetCategories();
		return fixedCategories.size() + electiveCategories.size();
	}

	static void RefreshRemainingElectiveCount()
	{
		auto skillCategoryCount = GetSkillCategoryCount();
		Terminal::WriteLine("Skill Categories Chosen: {}/{}", skillCategoryCount, scratch_pad::detailed_start::ElectiveSkillCategories::SKILL_CATEGORY_COUNT);
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
		return GetSkillCategoryCount() >= scratch_pad::detailed_start::ElectiveSkillCategories::SKILL_CATEGORY_COUNT;
	}

	static void OnDone()
	{
		scratch_pad::detailed_start::ProfessionalSkillPointAllocations::Clear();
		scratch_pad::detailed_start::ProfessionalSkillSet::GenerateSkills(
			scratch_pad::detailed_start::Profession::GetProfession(), 
			scratch_pad::detailed_start::ElectiveSkillCategories::GetCategories());
		application::UIState::Write(ChooseProfessionalSkill::GetStateId());
	}

	static void UpdateMenu();

	static std::function<void()> DoElectSkillCategory(const game::SkillCategory& category)
	{
		return [category]() 
		{
			scratch_pad::detailed_start::ElectiveSkillCategories::AddCategory(category);
			UpdateMenu();
			Refresh();
		};
	}

	static std::set<game::SkillCategory> GetAlreadyPresentSkillCategories()
	{
		std::set<game::SkillCategory> alreadyPresent;
		auto fixedCategories = GetFixedSkillCategories();
		auto electiveCategories = scratch_pad::detailed_start::ElectiveSkillCategories::GetCategories();
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
			auto allCategories = game::session::SkillCategories().GetSkillCategories();
			std::for_each(
				allCategories.begin(),
				allCategories.end(),
				[&alreadyPresent](const auto& category)
				{
					if (!alreadyPresent.contains(category.operator game::SkillCategory()))
					{
						Terminal::menu.AddAction({ category.GetName(), DoElectSkillCategory(category.operator game::SkillCategory()) });
					}
				});
		}
		MenuAction defaultAction = { "Never mind", application::UIState::DoGoTo(DetailedStart::GetStateId) };
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
		Registrar::Register(stateId, [](int currentState)
			{
				::application::OnEnter::AddHandler(currentState, OnEnter);
				::application::Renderer::SetRenderLayout(currentState, Terminal::LAYOUT_NAME);
				::application::Keyboard::AddHandler(
					currentState,
					Terminal::DoMenuInput(
						Terminal::INVALID_INPUT,
						Refresh));
			});
	}
}