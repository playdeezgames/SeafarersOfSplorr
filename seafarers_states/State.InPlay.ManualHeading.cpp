#include <Game.Session.h>
#include "State.InPlay.Globals.h"
#include "State.InPlay.ManualHeading.h"
#include "State.InPlay.ShipStatus.h"
#include "State.Registrar.h"
namespace state::in_play
{
	std::optional<int> ManualHeading::stateId = std::nullopt;

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("What is yer new heading?");
		Terminal::ShowPrompt();
	}

	static void OnEnter()
	{
		PlayMainTheme();
		Refresh();
	}

	static void OnValidInput(double value)
	{
		game::Session().GetPlayer().GetCharacter().GetBerth().GetShip().SetHeading(value);
		application::UIState::Write(ShipStatus::GetStateId());
	}

	void ManualHeading::Start()
	{
		Registrar::Register(stateId, [](int stateId) 
			{
				::application::OnEnter::AddHandler(stateId, OnEnter);
				::application::Renderer::SetRenderLayout(stateId, Terminal::LAYOUT_NAME);
				::application::Keyboard::AddHandler(
					stateId,
					Terminal::DoDoubleInput(
						application::UIState::GoTo(ShipStatus::GetStateId()),
						OnValidInput));
			});
	}
}