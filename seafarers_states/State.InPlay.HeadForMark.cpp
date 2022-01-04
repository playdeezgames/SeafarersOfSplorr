#include <algorithm>
#include <Common.Heading.h>
#include <Game.Session.h>
#include "State.InPlay.HeadForMark.h"
#include "State.InPlay.Globals.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_HEAD_FOR_MARK;

	static void Refresh()
	{
		Terminal::Reinitialize();

		auto playerCharacter =
			game::Session()
			.GetPlayer().GetCharacter();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Head for Marked Location");

		Terminal::ShowMenu();

		Terminal::ShowPrompt();
	}

	static std::function<void()> DoHeadForLocation(const common::XY<double>& destination)
	{
		return [destination]()
		{
			auto ship = game::Session().GetPlayer().GetCharacter().GetBerth().GetShip();
			auto relativeLocation = destination - ship.GetLocation();
			ship.SetHeading(common::Heading::XYToDegrees(relativeLocation));
		};
	}

	static void UpdateMenu()
	{
		auto playerCharacter =
			game::Session()
			.GetPlayer().GetCharacter();
		auto ship = playerCharacter.GetBerth().GetShip();
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);

		auto marks = playerCharacter.GetMarks().GetMarks();
		std::for_each(
			marks.begin(),
			marks.end(),
			[](const game::session::character::Mark& mark) 
			{
				Terminal::menu.AddAction({ mark.GetName(), DoHeadForLocation(mark.GetLocation()) });
			});
		MenuAction defaultAction = { "Never mind", application::UIState::GoTo(::UIState::IN_PLAY_CHANGE_HEADING) };
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
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoMenuInput(
				Terminal::INVALID_INPUT,
				Refresh));
	}
}