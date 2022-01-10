#include <algorithm>
#include <Common.Data.h>
#include <Game.Session.h>
#include <Game.ShipNames.h>
#include "State.InPlay.Globals.h"
#include "State.InPlay.RenameShipAdjective.h"
#include "State.InPlay.RenameShipNoun.h"
#include "State.ScratchPad.ShipAdjective.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_RENAME_SHIP_NOUN;

	static std::map<int, std::string> nouns;

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Select Noun:");

		Terminal::ShowMenu();

		Terminal::ShowPrompt();
	}

	static std::function<void()> DoChooseNoun(const std::string& noun)
	{
		return [noun]()
		{
			game::Session()
				.GetPlayer()
				.GetCharacter()
				.GetBerth()
				.GetShip()
				.SetName(std::format("{} {}", scratch_pad::ShipAdjective::GetAdjective(), noun));
			application::UIState::Write(::UIState::IN_PLAY_SHIP_STATUS);
		};
	}

	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		auto nouns = game::ShipNames::GetNouns();
		std::for_each(
			nouns.begin(),
			nouns.end(),
			[](const std::string& noun)
			{
				Terminal::menu.AddAction({ noun, DoChooseNoun(noun) });
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

	void RenameShipNoun::Start()
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