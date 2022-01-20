#include <Game.Session.h>
#include <Game.Session.World.h>
#include "State.InPlay.Globals.h"
#include "State.ChooseStartType.h"
#include "State.DetailedStart.h"
#include "State.Registrar.h"
#include "State.StartGame.h"
#include "State.Terminal.h"
#include "State.Tip.h"
#include "State.ScratchPad.DetailedStart.Profession.h"
#include "State.ScratchPad.DetailedStart.ElectiveSkillCategories.h"
#include "State.ScratchPad.DetailedStart.ProfessionalSkillPointAllocations.h"
namespace state
{
	std::optional<int> ChooseStartType::stateId = std::nullopt;

	static void Refresh()
	{
		Terminal::ClearStatusLine();
		Terminal::ClearInput();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine();
		Terminal::WriteLine("Choose Start Type:");

		Terminal::ShowMenu();

		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine();
		Terminal::Write(">");
	}

	static void OnQuickStart()
	{
		scratch_pad::detailed_start::Profession::SetProfession(game::Profession::SAILOR);
		scratch_pad::detailed_start::ElectiveSkillCategories::Clear();
		scratch_pad::detailed_start::ElectiveSkillCategories::AddCategory(game::session::SkillCategory(game::SkillCategory::LISTEN));
		scratch_pad::detailed_start::ElectiveSkillCategories::AddCategory(game::session::SkillCategory(game::SkillCategory::REPAIR));
		scratch_pad::detailed_start::ElectiveSkillCategories::AddCategory(game::session::SkillCategory(game::SkillCategory::SPOT));
		auto skills = game::session::World().GetSkills();
		auto skillId = skills.GetSkillsInCategory(game::SkillCategory::CLIMB).begin()->operator int();
		scratch_pad::detailed_start::ProfessionalSkillPointAllocations::AllocateSkillPoints(skillId, 50);
		skillId = skills.GetSkillsInCategory(game::SkillCategory::DODGE).begin()->operator int();
		scratch_pad::detailed_start::ProfessionalSkillPointAllocations::AllocateSkillPoints(skillId, 50);
		skillId = skills.GetSkillsInCategory(game::SkillCategory::NAVIGATE).begin()->operator int();
		scratch_pad::detailed_start::ProfessionalSkillPointAllocations::AllocateSkillPoints(skillId, 50);
		skillId = skills.GetSkillsInCategory(game::SkillCategory::SWIM).begin()->operator int();
		scratch_pad::detailed_start::ProfessionalSkillPointAllocations::AllocateSkillPoints(skillId, 50);
		skillId = skills.GetSkillsInCategory(game::SkillCategory::LISTEN).begin()->operator int();
		scratch_pad::detailed_start::ProfessionalSkillPointAllocations::AllocateSkillPoints(skillId, 50);
		skillId = skills.GetSkillsInCategory(game::SkillCategory::SPOT).begin()->operator int();
		scratch_pad::detailed_start::ProfessionalSkillPointAllocations::AllocateSkillPoints(skillId, 50);
		game::Session()
			.GetPlayer()
			.Populate(
				game::session::World().GetDifficulty(),
				scratch_pad::detailed_start::ProfessionalSkillPointAllocations::GetAllocations());
		application::UIState::Write(Tip::GetStateId());
	}

	static void OnDetailedStart()
	{
		application::UIState::Write(DetailedStart::GetStateId());
	}

	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		Terminal::menu.AddAction({ "Quick Start", OnQuickStart });
		Terminal::menu.AddAction({ "Detailed Start", OnDetailedStart });
		MenuAction defaultAction = { "Never mind", application::UIState::DoGoTo(StartGame::GetStateId) };
		Terminal::menu.SetDefaultAction(defaultAction);
	}

	static void OnEnter()
	{
		in_play::PlayMainTheme();
		UpdateMenu();
		Refresh();
	}

	static void DoStart(int stateId)
	{
		::application::OnEnter::AddHandler(stateId, OnEnter);
		::application::Renderer::SetRenderLayout(stateId, Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			stateId,
			Terminal::DoMenuInput(
				Terminal::INVALID_INPUT,
				Refresh));
	}

	void ChooseStartType::Start()
	{
		Registrar::Register(stateId, DoStart);
	}
}