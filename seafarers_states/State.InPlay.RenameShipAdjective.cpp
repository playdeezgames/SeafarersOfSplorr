#include <algorithm>
#include <Common.Data.h>
#include <Game.Session.Player.h>
#include <Game.ShipNames.h>
#include "State.InPlay.Globals.h"
#include "State.InPlay.RenameShipAdjective.h"
#include "State.ScratchPad.ShipAdjective.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_RENAME_SHIP_ADJECTIVE;

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Select Adjective:");

		Terminal::ShowMenu();

		Terminal::ShowPrompt();
	}

	static std::function<void()> DoChooseAdjective(const std::string& adjective)
	{
		return [adjective]() 
		{
			scratch_pad::ShipAdjective::SetAdjective(adjective);
			application::UIState::Write(::UIState::IN_PLAY_RENAME_SHIP_NOUN);
		};
	}

	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		auto adjectives = game::ShipNames::GetAdjectives();
		std::for_each(
			adjectives.begin(), 
			adjectives.end(), 
			[](const std::string& adjective) 
			{
				Terminal::menu.AddAction({ adjective, DoChooseAdjective(adjective) });
			});
		MenuAction defaultAction = { "Never mind", application::UIState::GoTo(::UIState::IN_PLAY_SHIP_STATUS) };
		Terminal::menu.SetDefaultAction(defaultAction);
	}

	static void OnEnter()
	{
		PlayMainTheme();
		UpdateMenu();
		Refresh();
	}

	void RenameShipAdjective::Start()
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