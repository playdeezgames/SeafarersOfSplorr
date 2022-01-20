#include <Game.Session.h>
#include <Game.Session.Characters.h>
#include "State.InPlay.CrewDetail.h"
#include "State.InPlay.CrewDetail.Characteristics.h"
#include "State.InPlay.Globals.h"
#include "State.Registrar.h"
#include "State.ScratchPad.CrewDetail.h"
namespace state::in_play::crew_detail
{
	std::optional<int> Characteristics::stateId = std::nullopt;

	static auto OnLeave = application::UIState::DoGoTo(CrewDetail::GetStateId);

	static void Refresh()
	{
		auto character =
			game::session::Characters()
			.GetCharacter(scratch_pad::CrewDetail::GetCharacterId());
		auto characteristics =
			character
			.GetCharacteristics()
			.GetAll();
		auto hitPoints =
			character
			.GetHitpoints();

		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Characteristics:");

		Terminal::SetForeground(game::Colors::GRAY);
		for (auto characteristic : characteristics)
		{
			Terminal::WriteLine("{}: {}", characteristic.GetName(), characteristic.GetValue());
		}

		Terminal::WriteLine("HP: {}/{}", hitPoints.GetCurrent(), hitPoints.GetMaximum());

		Terminal::ShowMenu();

		Terminal::ShowPrompt();
	}

	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		MenuAction defaultAction = { "Never mind", OnLeave };
		Terminal::menu.SetDefaultAction(defaultAction);
	}

	static void OnEnter()
	{
		PlayMainTheme();
		UpdateMenu();
		Refresh();
	}

	void Characteristics::Start()
	{
		Registrar::Register(stateId, [](int stateId) 
			{
				::application::OnEnter::AddHandler(
					stateId,
					OnEnter);
				::application::Renderer::SetRenderLayout(
					stateId,
					Terminal::LAYOUT_NAME);
				::application::Keyboard::AddHandler(
					stateId,
					Terminal::DoMenuInput(
						Terminal::INVALID_INPUT,
						Refresh));
			});
	}
}