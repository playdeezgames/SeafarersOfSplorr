#include <Game.Session.h>
#include "State.InPlay.ChangeSpeed.h"
#include "State.InPlay.Globals.h"
#include "State.Registrar.h"
namespace state::in_play
{
	std::optional<int> ChangeSpeed::stateId = std::nullopt;

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Change Speed:");
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine("Current Speed: {:.2f}", 
			game::Session()
			.GetPlayer().GetCharacter()
			.GetBerth()
			.GetShip()
			.GetSpeed());

		Terminal::ShowMenu();

		Terminal::ShowPrompt();
	}

	static std::function<void()> DoSetSpeed(double speed)
	{
		return [speed]() 
		{
			game::Session()
				.GetPlayer().GetCharacter()
				.GetBerth()
				.GetShip()
				.SetSpeed(speed);
			application::UIState::Write(::UIState::IN_PLAY_SHIP_STATUS);
		};
	}

	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		Terminal::menu.AddAction({ "All stop", DoSetSpeed(0.0) });
		Terminal::menu.AddAction({ "Ahead 1/3", DoSetSpeed(0.3) });
		Terminal::menu.AddAction({ "Ahead 2/3", DoSetSpeed(0.6) });
		Terminal::menu.AddAction({ "Ahead Full", DoSetSpeed(0.9) });
		Terminal::menu.AddAction({ "Ahead Flank", DoSetSpeed(1.0) });
		MenuAction defaultAction = { "Never mind", application::UIState::GoTo(::UIState::IN_PLAY_SHIP_STATUS) };
		Terminal::menu.SetDefaultAction(defaultAction);
	}

	static void OnEnter()
	{
		PlayMainTheme();
		UpdateMenu();
		Refresh();
	}

	void ChangeSpeed::Start()
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