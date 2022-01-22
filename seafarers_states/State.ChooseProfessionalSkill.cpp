#include "State.ChooseProfessionalSkill.h"
#include "State.ChooseElectiveSkillCategories.h"
#include "State.ChoosePersonalSkill.h"
#include "State.InPlay.Globals.h"
#include "State.ScratchPad.DetailedStart.PersonalSkillPointAllocations.h"
#include "State.ScratchPad.DetailedStart.ProfessionalSkillPointAllocations.h"
#include "State.ScratchPad.DetailedStart.ProfessionalSkillSet.h"
#include "State.ScratchPad.SelectedSkill.h"
#include "State.SpendProfessionalSkillPoints.h"
#include <algorithm>
#include <Game.Session.Player.h>
#include <Game.Session.World.h>
#include <Game.Session.World.Skills.h>
namespace state
{
	std::optional<int> ChooseProfessionalSkill::stateId = std::nullopt;

	static void RefreshExistingSkillPointAllocations()
	{
		auto allocations = scratch_pad::detailed_start::ProfessionalSkillPointAllocations::GetAllocations();
		std::for_each(
			allocations.begin(),
			allocations.end(),
			[](const std::pair<int,size_t>& entry) 
			{
				auto skillId = entry.first;
				auto allocation = entry.second;
				auto skill = game::session::world::Skills::GetSkill(skillId);
				Terminal::WriteLine("{}: {}", skill.GetName(), allocation);
			});
	}

	static void RefreshAllocationSummary()
	{
		Terminal::WriteLine(
			"Points allocated: {}/{}", 
			scratch_pad::detailed_start::ProfessionalSkillPointAllocations::GetSkillPointsAllocated(),
			scratch_pad::detailed_start::ProfessionalSkillPointAllocations::SKILL_POINT_COUNT);
	}

	static void Refresh()
	{
		Terminal::ClearStatusLine();
		Terminal::ClearInput();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine();
		Terminal::WriteLine("Allocate Professional Skill Points:");

		Terminal::SetForeground(game::Colors::GRAY);
		RefreshExistingSkillPointAllocations();
		RefreshAllocationSummary();

		Terminal::ShowMenu();

		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine();
		Terminal::Write(">");
	}

	static std::function<void()> DoChooseSkill(int skillId)
	{
		return [skillId]() 
		{
			scratch_pad::SelectedSkill::SetSkillId(skillId);
			application::UIState::Write(SpendProfessionalSkillPoints::GetStateId());
		};
	}

	static void OnDone()
	{
		game::session::Player::Populate(
				game::session::World::GetDifficulty(),
				scratch_pad::detailed_start::ProfessionalSkillPointAllocations::GetAllocations());
		scratch_pad::detailed_start::PersonalSkillPointAllocations::Clear();
		application::UIState::Write(ChoosePersonalSkill::GetStateId());
	}

	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		if (scratch_pad::detailed_start::ProfessionalSkillPointAllocations::GetSkillPointsRemaining() == 0)
		{
			Terminal::menu.AddAction({ "Done", OnDone });
		}
		else
		{
			auto skills = scratch_pad::detailed_start::ProfessionalSkillSet::GetSkills();
			std::for_each(
				skills.begin(), 
				skills.end(), 
				[](const game::session::Skill& skill) 
				{
					Terminal::menu.AddAction({ skill.GetName(), DoChooseSkill(skill.ToId()) });
				});
		}
		MenuAction defaultAction = { "Never mind", application::UIState::DoGoTo(ChooseElectiveSkillCategories::GetStateId) };
		Terminal::menu.SetDefaultAction(defaultAction);
	}

	static void OnEnter()
	{
		in_play::PlayMainTheme();
		UpdateMenu();
		Refresh();
	}

	void ChooseProfessionalSkill::Start()
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