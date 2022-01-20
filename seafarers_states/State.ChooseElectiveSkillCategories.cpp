#include <algorithm>
#include <Game.Session.h>
#include <iterator>
#include <set>
#include "State.DetailedStart.h"
#include "State.InPlay.Globals.h"
#include "State.Terminal.h"
#include "State.ChooseElectiveSkillCategories.h"
#include "State.ChooseProfessionalSkill.h"
#include "State.Registrar.h"
#include "State.ScratchPad.DetailedStart.Profession.h"
#include "State.ScratchPad.DetailedStart.ElectiveSkillCategories.h"
#include "State.ScratchPad.DetailedStart.ProfessionalSkillPointAllocations.h"
#include "State.ScratchPad.DetailedStart.ProfessionalSkillSet.h"
namespace state
{
	std::optional<int> ChooseElectiveSkillCategories::stateId = std::nullopt;

	static std::set<game::session::SkillCategory> GetFixedSkillCategories()
	{
		return game::Session()
			.GetWorld()
			.GetProfessions()
			.GetProfession(scratch_pad::detailed_start::Profession::GetProfession())
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
		auto electiveCategories = scratch_pad::detailed_start::ElectiveSkillCategories::GetCategories();
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

	static std::function<void()> DoElectSkillCategory(const game::session::SkillCategory& category)
	{
		return [category]() 
		{
			scratch_pad::detailed_start::ElectiveSkillCategories::AddCategory(category);
			UpdateMenu();
			Refresh();
		};
	}

	static std::set<game::session::SkillCategory> GetAlreadyPresentSkillCategories()
	{
		std::set<game::session::SkillCategory> alreadyPresent;
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
		Registrar::Register(stateId, [](int stateId) 
			{
				::application::OnEnter::AddHandler(stateId, OnEnter);
				::application::Renderer::SetRenderLayout(stateId, Terminal::LAYOUT_NAME);
				::application::Keyboard::AddHandler(
					stateId,
					Terminal::DoMenuInput(
						Terminal::INVALID_INPUT,
						Refresh));
			});
	}
}