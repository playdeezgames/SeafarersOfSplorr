#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Game.Audio.Mux.h>
#include <Game.Avatar.Actions.h>
#include <Game.Colors.h>
#include "State.InPlay.Shipyard.h"
#include "State.Terminal.h"
#include "UIState.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_SHIPYARD;

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Shipyard:");

		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine("1) Purchase ship");
		Terminal::WriteLine("2) Unfoul ship");
		Terminal::WriteLine("0) Leave");

		Terminal::ShowPrompt();

	}

	static void OnEnter()
	{
		Refresh();
	}

	static void OnLeave()
	{
		game::avatar::Actions::DoAction(game::avatar::Action::ENTER_DOCK);
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnPurchaseShip()
	{
		::application::UIState::Write(::UIState::IN_PLAY_SHIPYARD_PURCHASE_SHIP);
	}

	static void OnUnfoulShip()
	{
		::application::UIState::Write(::UIState::IN_PLAY_SHIPYARD_UNFOUL_SHIP);
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{ "1", OnPurchaseShip},
		{ "2", OnUnfoulShip},
		{ "0", OnLeave}
	};

	void Shipyard::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoIntegerInput(
				menuActions,
				Terminal::INVALID_INPUT,
				Refresh));
	}
}