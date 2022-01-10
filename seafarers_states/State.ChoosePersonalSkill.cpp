#include <algorithm>
#include <Game.Session.h>
#include "State.InPlay.Globals.h"
#include "State.ChoosePersonalSkill.h"
#include "State.ScratchPad.SelectedSkill.h"
#include "State.ScratchPad.DetailedStart.PersonalSkillPointAllocations.h"
namespace state
{
	static const ::UIState CURRENT_STATE = ::UIState::CHOOSE_PERSONAL_SKILL;

	static void RefreshExistingSkillPointAllocations()
	{
		Terminal::WriteLine(
			"Points allocated: {}/{}",
			scratch_pad::detailed_start::PersonalSkillPointAllocations::GetSkillPointsAllocated(),
			scratch_pad::detailed_start::PersonalSkillPointAllocations::GetSkillPointCount());
	}

	static void RefreshAllocationSummary()
	{

	}

	static void Refresh()
	{
		Terminal::ClearStatusLine();
		Terminal::ClearInput();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine();
		Terminal::WriteLine("Allocate Personal Skill Points:");

		Terminal::SetForeground(game::Colors::GRAY);
		RefreshExistingSkillPointAllocations();
		RefreshAllocationSummary();

		Terminal::ShowMenu();

		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine();
		Terminal::Write(">");
	}

	static void OnDone()
	{
		auto personalSkillPointAllocations = scratch_pad::detailed_start::PersonalSkillPointAllocations::GetSkillPointAllocations();
		std::for_each(
			personalSkillPointAllocations.begin(),
			personalSkillPointAllocations.end(),
			[](const std::pair<int, size_t>& entry)
			{
				game::Session()
					.GetPlayer()
					.GetCharacter()
					.GetSkills()
					.GetSkill(entry.first)
					.AllocatePoints(entry.second);
			});
		application::UIState::Write(::UIState::TIP);
	}

	static std::function<void()> DoChooseSkill(int skillId)
	{
		return [skillId]() 
		{
			scratch_pad::SelectedSkill::SetSkillId(skillId);
			application::UIState::Write(::UIState::SPEND_PERSONAL_SKILL_POINTS);
		};
	}

	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		if (scratch_pad::detailed_start::PersonalSkillPointAllocations::GetSkillPointsRemaining() == 0)
		{
			Terminal::menu.AddAction({ "Done", OnDone });
		}
		else
		{
			auto skills = game::Session().GetWorld().GetSkills().GetSkills();
			std::for_each(
				skills.begin(),
				skills.end(),
				[](const game::session::Skill& skill)
				{
					Terminal::menu.AddAction({ skill.GetName(), DoChooseSkill(skill.operator int()) });
				});
		}
	}

	static void OnEnter()
	{
		in_play::PlayMainTheme();
		UpdateMenu();
		Refresh();
	}

	void ChoosePersonalSkill::Start()
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