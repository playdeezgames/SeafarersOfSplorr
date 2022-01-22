#include "State.DetailedStart.h"
#include "State.ChooseElectiveSkillCategories.h"
#include "State.ChooseStartType.h"
#include "State.InPlay.Globals.h"
#include "State.ScratchPad.DetailedStart.ElectiveSkillCategories.h"
#include "State.ScratchPad.DetailedStart.Profession.h"
#include <algorithm>
#include <Game.Session.World.Professions.h>
namespace state
{
	std::optional<int> DetailedStart::stateId = std::nullopt;

	static void Refresh()
	{
		Terminal::ClearStatusLine();
		Terminal::ClearInput();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine();
		Terminal::WriteLine("Choose Profession:");

		Terminal::ShowMenu();

		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine();
		Terminal::Write(">");
	}

	static std::function<void()> DoSelectProfession(const game::Profession& profession)
	{
		return [profession]()
		{
			scratch_pad::detailed_start::Profession::SetProfession(profession);
			scratch_pad::detailed_start::ElectiveSkillCategories::Clear();
			application::UIState::Write(ChooseElectiveSkillCategories::GetStateId());
		};
	}

	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		auto professions = game::session::world::Professions::GetProfessions();
		std::for_each(
			professions.begin(),
			professions.end(),
			[](const auto& profession) 
			{
				Terminal::menu.AddAction({ profession.GetName(), DoSelectProfession(profession.operator game::Profession()) });
			});
		MenuAction defaultAction = { "Never mind", application::UIState::DoGoTo(ChooseStartType::GetStateId) };
		Terminal::menu.SetDefaultAction(defaultAction);
	}

	static void OnEnter()
	{
		in_play::PlayMainTheme();
		UpdateMenu();
		Refresh();
	}

	void DetailedStart::Start()
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