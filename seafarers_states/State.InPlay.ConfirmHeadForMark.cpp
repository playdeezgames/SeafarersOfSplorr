#include <Common.Heading.h>
#include <Game.Session.h>
#include "State.InPlay.ConfirmHeadForMark.h"
#include "State.InPlay.Globals.h"
#include "State.InPlay.ShipStatus.h"
#include "State.ScratchPad.HeadForMark.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_CONFIRM_HEAD_FOR_MARK;

	static void Refresh()
	{
		Terminal::Reinitialize();

		auto playerCharacter =
			game::Session()
			.GetPlayer()
			.GetCharacter();
		auto ship =
			playerCharacter
			.GetBerth()
			.GetShip();
		auto mark = 
			playerCharacter
			.GetMarks()
			.GetMark(scratch_pad::HeadForMark::GetMark());

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Confirm Heading or Delete Mark");
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine("Mark Name: {}", mark.GetName());

		auto relativeLocation = mark.GetLocation() - ship.GetLocation();
		Terminal::WriteLine("Distance: {:.2f}", relativeLocation.GetMagnitude());
		Terminal::WriteLine("Heading: {:.2f}\xf8", common::Heading::XYToDegrees(relativeLocation));

		Terminal::ShowMenu();

		Terminal::ShowPrompt();
	}

	static void OnConfirm()
	{
		auto playerCharacter =
			game::Session()
			.GetPlayer()
			.GetCharacter();
		auto ship =
			playerCharacter
			.GetBerth()
			.GetShip();
		auto mark =
			playerCharacter
			.GetMarks()
			.GetMark(scratch_pad::HeadForMark::GetMark());

		auto relativeLocation = mark.GetLocation() - ship.GetLocation();
		ship.SetHeading(common::Heading::XYToDegrees(relativeLocation));

		application::UIState::Write(ShipStatus::GetStateId());
	}

	static void OnDelete()
	{
		game::Session()
			.GetPlayer()
			.GetCharacter()
			.GetMarks()
			.RemoveMark(scratch_pad::HeadForMark::GetMark());

		application::UIState::Write(ShipStatus::GetStateId());
	}

	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		Terminal::menu.AddAction({ "Confirm Heading", OnConfirm});
		Terminal::menu.AddAction({ "Delete Mark", OnDelete });
		MenuAction defaultAction = { "Never mind", application::UIState::GoTo(::UIState::IN_PLAY_HEAD_FOR_MARK) };
		Terminal::menu.SetDefaultAction(defaultAction);
	}

	static void OnEnter()
	{
		PlayMainTheme();
		UpdateMenu();
		Refresh();
	}

	void ConfirmHeadForMark::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoMenuInput(
				Terminal::INVALID_INPUT,
				Refresh));
	}
}