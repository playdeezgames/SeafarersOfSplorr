#include <algorithm>
#include <Game.Session.h>
#include "State.InPlay.Globals.h"
#include "State.AllocateProfessionalSkillPoints.h"
namespace state
{
	static ::UIState CURRENT_STATE = ::UIState::ALLOCATE_PROFESSIONAL_SKILL_POINTS;

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

	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		MenuAction defaultAction = { "Never mind", application::UIState::GoTo(::UIState::CHOOSE_ELECTIVE_SKILL_CATEGORIES) };
		Terminal::menu.SetDefaultAction(defaultAction);
	}

	static void OnEnter()
	{
		in_play::PlayMainTheme();
		UpdateMenu();
		Refresh();
	}

	void AllocateProfessionalSkillPoints::Start()
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