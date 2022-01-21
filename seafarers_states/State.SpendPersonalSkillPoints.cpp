#include "State.SpendPersonalSkillPoints.h"
#include "State.ChoosePersonalSkill.h"
#include "State.InPlay.Globals.h"
#include "State.ScratchPad.DetailedStart.PersonalSkillPointAllocations.h"
#include "State.ScratchPad.SelectedSkill.h"
#include <algorithm>
#include <Common.Data.h>
#include <Game.Session.World.Skills.h>
namespace state
{
	std::optional<int> SpendPersonalSkillPoints::stateId = std::nullopt;

	static void Refresh()
	{
		Terminal::ClearStatusLine();
		Terminal::ClearInput();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine();
		Terminal::WriteLine("How many Skill Points?");

		Terminal::SetForeground(game::Colors::GRAY);
		auto allocations = scratch_pad::detailed_start::PersonalSkillPointAllocations::GetSkillPointAllocations();
		std::for_each(
			allocations.begin(),
			allocations.end(),
			[](const std::pair<int, size_t>& entry)
			{
				auto skill = game::session::world::Skills().GetSkill(entry.first);
				Terminal::WriteLine("{} ({})", skill.GetName(), entry.second);
			});
		Terminal::WriteLine(
			"Skill Points Allocated: {}/{}", 
			scratch_pad::detailed_start::PersonalSkillPointAllocations::GetSkillPointsAllocated(), 
			scratch_pad::detailed_start::PersonalSkillPointAllocations::GetSkillPointCount());

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
		{ "0", application::UIState::DoGoTo(ChoosePersonalSkill::GetStateId)}
	};


	static void OnOtherInput(const std::string& text)
	{
		size_t points = (size_t)common::Data::ToInt(text);
		if (points <= scratch_pad::detailed_start::PersonalSkillPointAllocations::GetSkillPointsRemaining())
		{
			scratch_pad::detailed_start::PersonalSkillPointAllocations::AllocateSkillPoints(scratch_pad::SelectedSkill::GetSkillId(), points);
			application::UIState::Write(ChoosePersonalSkill::GetStateId());
		}
		else
		{
			Terminal::ErrorMessage("You don't have enough skill points remaining!");
			Refresh();
		}
	}

	void SpendPersonalSkillPoints::Start()
	{
		Registrar::Register(stateId, [](int stateId) 
			{
				::application::OnEnter::AddHandler(stateId, OnEnter);
				::application::Renderer::SetRenderLayout(stateId, Terminal::LAYOUT_NAME);
				::application::Keyboard::AddHandler(
					stateId,
					Terminal::DoIntegerInput(
						menuActions,
						OnOtherInput));
			});
	}
}