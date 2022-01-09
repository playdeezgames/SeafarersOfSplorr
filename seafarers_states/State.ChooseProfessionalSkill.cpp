#include <algorithm>
#include <Game.Session.h>
#include "State.InPlay.Globals.h"
#include "State.ChooseProfessionalSkill.h"
namespace state
{
	static const ::UIState CURRENT_STATE = ::UIState::CHOOSE_PROFESSIONAL_SKILL;

	static void RefreshExistingSkillPointAllocations()
	{
		auto allocations = game::session::Player::GetProfessionSkillPointAllocations();
		std::for_each(
			allocations.begin(),
			allocations.end(),
			[](const std::pair<int,size_t>& entry) 
			{
				auto skillId = entry.first;
				auto allocation = entry.second;
				auto skill = game::Session().GetWorld().GetSkills().GetSkill(skillId);
				Terminal::WriteLine("{}: {}", skill.GetName(), allocation);
			});
	}

	static void RefreshAllocationSummary()
	{
		Terminal::WriteLine(
			"Points allocated: {}/{}", 
			game::session::Player::GetProfessionalSkillPointsAllocated(), 
			game::session::Player::PROFESSIONAL_SKILL_POINT_COUNT);
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
			game::session::Player::SetSelectedSkillId(skillId);
			application::UIState::Write(::UIState::SPEND_PROFESSIONAL_SKILL_POINTS);
		};
	}

	static void OnDone()
	{

	}

	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		if (game::session::Player::GetProfessionalSkillPointsRemaining() == 0)
		{
			Terminal::menu.AddAction({ "Done", OnDone });
		}
		else
		{
			//TODO: if no skill points remain to allocate, only "done" is available
			auto skills = game::session::Player::GetProfessionalSkillSet();
			std::for_each(
				skills.begin(), 
				skills.end(), 
				[](const game::session::Skill& skill) 
				{
					Terminal::menu.AddAction({ skill.GetName(), DoChooseSkill(skill.operator int()) });
				});
		}
		MenuAction defaultAction = { "Never mind", application::UIState::GoTo(::UIState::CHOOSE_ELECTIVE_SKILL_CATEGORIES) };
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
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoMenuInput(
				Terminal::INVALID_INPUT,
				Refresh));
	}
}