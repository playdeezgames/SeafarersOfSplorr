#include <algorithm>
#include <Game.Session.h>
#include "State.InPlay.Globals.h"
#include "State.ChooseStartType.h"
#include "State.Terminal.h"
#include "State.DetailedStart.h"
#include "State.Registrar.h"
#include "State.ScratchPad.DetailedStart.Profession.h"
#include "State.ScratchPad.DetailedStart.ElectiveSkillCategories.h"
#include "UIState.h"
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
			application::UIState::Write(::UIState::CHOOSE_ELECTIVE_SKILL_CATEGORIES);
		};
	}

	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		auto professions = game::Session().GetWorld().GetProfessions().GetProfessions();
		std::for_each(
			professions.begin(),
			professions.end(),
			[](const game::session::world::Profession& profession) 
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