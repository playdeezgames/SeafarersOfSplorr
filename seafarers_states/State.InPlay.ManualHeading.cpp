#include "State.InPlay.ManualHeading.h"
#include "State.InPlay.Globals.h"
#include "State.InPlay.ShipStatus.h"
#include <Game.Session.Player.h>
#include <Game.Session.Character.h>
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
		game::session::Character(game::session::Player::GetCharacterId()).GetBerth().GetShip().SetHeading(value);
		application::UIState::Write(ShipStatus::GetStateId());
	}

	void ManualHeading::Start()
	{
		Registrar::Register(stateId, [](int currentState)
			{
				::application::OnEnter::AddHandler(currentState, OnEnter);
				::application::Renderer::SetRenderLayout(currentState, Terminal::LAYOUT_NAME);
				::application::Keyboard::AddHandler(
					currentState,
					Terminal::DoDoubleInput(
						application::UIState::GoTo(ShipStatus::GetStateId()),
						OnValidInput));
			});
	}
}