#include "State.InPlay.Ship.HuntRats.h"
#include "State.InPlay.Globals.h"
#include "State.Registrar.h"
#include "State.InPlay.ShipStatus.h"
namespace state::in_play::ship
{
	std::optional<int> HuntRats::stateId = std::nullopt;

	static void Refresh()
	{
		Terminal::Reinitialize();
		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Hunting Rats:");
		Terminal::SetForeground(game::Colors::GRAY);
		//TODO: the stuff
		Terminal::ShowMenu();
		Terminal::ShowPrompt();
	}

	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		//TODO: the stuff
		MenuAction defaultAction = { "Never mind", application::UIState::DoGoTo(ShipStatus::GetStateId) };
		Terminal::menu.SetDefaultAction(defaultAction);

	}

	static void OnEnter()
	{
		PlayMainTheme();
		UpdateMenu();
		Refresh();
	}

	void HuntRats::Start()
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