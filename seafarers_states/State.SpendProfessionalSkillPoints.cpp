#include <algorithm>
#include <Common.Data.h>
#include <Game.Session.h>
#include "State.InPlay.Globals.h"
#include "State.SpendProfessionalSkillPoints.h"
#include "State.ScratchPad.SelectedSkill.h"
namespace state
{
	static const ::UIState CURRENT_STATE = ::UIState::SPEND_PROFESSIONAL_SKILL_POINTS;

	static void Refresh()
	{
		Terminal::ClearStatusLine();
		Terminal::ClearInput();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine();
		Terminal::WriteLine("How many Skill Points?");

		Terminal::SetForeground(game::Colors::GRAY);
		auto allocations = game::session::Player::GetProfessionSkillPointAllocations();
		std::for_each(
			allocations.begin(),
			allocations.end(),
			[](const std::pair<int, size_t>& entry)
			{
				auto skill = game::Session().GetWorld().GetSkills().GetSkill(entry.first);
				Terminal::WriteLine("{} ({})", skill.GetName(), entry.second);
			});
		Terminal::WriteLine("Skill Points Allocated: {}/{}", game::session::Player::GetProfessionalSkillPointsRemaining(), game::session::Player::PROFESSIONAL_SKILL_POINT_COUNT);

		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine();
		Terminal::Write(">");
	}

	static void OnEnter()
	{
		in_play::PlayMainTheme();
		Refresh();
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{ "0", application::UIState::GoTo(::UIState::CHOOSE_PROFESSIONAL_SKILL)}
	};

	static void OnOtherInput(const std::string& text)
	{
		size_t points = (size_t)common::Data::ToInt(text);
		if (points <= game::session::Player::GetProfessionalSkillPointsRemaining())
		{
			game::session::Player::AllocateProfessionalSkillPoints(scratch_pad::SelectedSkill::GetSkillId(), points);
			application::UIState::Write(::UIState::CHOOSE_PROFESSIONAL_SKILL);
		}
		else
		{
			Terminal::ErrorMessage("You don't have enough skill points remaining!");
			Refresh();
		}
	}

	void SpendProfessionalSkillPoints::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoIntegerInput(
				menuActions,
				OnOtherInput));
	}
}