#include "State.InPlay.HeadForMark.h"
#include "State.InPlay.ChangeHeading.h"
#include "State.InPlay.ConfirmHeadForMark.h"
#include "State.InPlay.Globals.h"
#include "State.ScratchPad.HeadForMark.h"
#include <algorithm>
#include <Game.Session.Player.h>
#include <Game.Session.Character.h>
#include <Game.Session.Character.Marks.h>
#include <Game.Session.Character.Berth.h>
#include <Game.Session.Ship.h>
namespace state::in_play
{
	std::optional<int> HeadForMark::stateId = std::nullopt;

	static void Refresh()
	{
		Terminal::Reinitialize();

		auto playerCharacter =
			game::session::Character(game::session::Player::GetCharacterId());

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Head for Marked Location");

		Terminal::ShowMenu();

		Terminal::ShowPrompt();
	}

	static std::function<void()> DoHeadForLocation(const std::string& markName)
	{
		return [markName]()
		{
			scratch_pad::HeadForMark::SetMark(markName);
			application::UIState::Write(ConfirmHeadForMark::GetStateId());
		};
	}

	static void UpdateMenu()
	{
		auto playerCharacter =
			game::session::Character(game::session::Player::GetCharacterId());
		auto ship = 
			game::session::Ship(
			game::session::character::Berth(game::session::Player::GetCharacterId())
			.GetShipId());
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);

		auto marks = game::session::character::Marks(playerCharacter.ToId()).GetMarks();
		std::for_each(
			marks.begin(),
			marks.end(),
			[](const game::session::character::Mark& mark) 
			{
				Terminal::menu.AddAction({ std::string(mark.GetName()), DoHeadForLocation(std::string(mark.GetName())) });
			});
		MenuAction defaultAction = { "Never mind", application::UIState::DoGoTo(ChangeHeading::GetStateId) };
		Terminal::menu.SetDefaultAction(defaultAction);
	}

	static void OnEnter()
	{
		PlayMainTheme();
		UpdateMenu();
		Refresh();
	}

	void HeadForMark::Start()
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